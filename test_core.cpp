/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).
 * 
 * SPECIAL RESTRICTION: No use of this code and files (artifacts) is permitted 
 * for the training of machine learning models or artificial intelligence 
 * without explicit written permission.
 * 
 * COMMERCIAL CLAUSE: Any enterprise deployment requires a paid commercial license.
 * Full license text is available in the LICENSE file in the root directory.
 */

#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdint>

uint64_t vcore_exabit_sync(int exp) {
    if (exp <= 0) return 1;
    uint64_t n = static_cast<uint64_t>(std::abs(exp)) % 60; 
    return (3ULL << n) + 1; 
}

float apply_vcore_compression(float value) {
    if (std::abs(value) < 1e-9f) return 0.0f;
    int exp;
    float frac = std::frexp(value, &exp);
    if (exp > 1) {
        uint64_t sync = vcore_exabit_sync(exp);
        frac = (3.0f * frac * (sync % 1000) / 1000.0f) / 4.0f;
    }
    return std::ldexp(frac, exp);
}

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;
    int N = std::stoi(argv[1]);
    int M = std::stoi(argv[2]);

    try {
        std::vector<float> h_Q(N * N);
        std::ifstream input_file("temp_weights.bin", std::ios::binary);
        if (!input_file) return 1;
        input_file.read(reinterpret_cast<char*>(h_Q.data()), N * N * sizeof(float));
        input_file.close();

        for (float& val : h_Q) val = apply_vcore_compression(val);

        MarkovAI::LinearSelector selector(N, M, h_Q);

        std::vector<float> h_input(N);
        std::ifstream input_x("temp_x.bin", std::ios::binary);
        input_x.read(reinterpret_cast<char*>(h_input.data()), N * sizeof(float));
        input_x.close();

        float *d_input, *d_output;
        cudaMalloc(&d_input, N * sizeof(float));
        cudaMalloc(&d_output, N * sizeof(float));
        cudaMemcpy(d_input, h_input.data(), N * sizeof(float), cudaMemcpyHostToDevice);

        selector.select_projection_gpu(d_input, d_output);

        std::vector<float> h_output(N);
        cudaMemcpy(h_output.data(), d_output, N * sizeof(float), cudaMemcpyDeviceToHost);

        for (float& val : h_output) val = apply_vcore_compression(val);

        std::ofstream output_file("temp_out.bin", std::ios::binary);
        output_file.write(reinterpret_cast<char*>(h_output.data()), N * sizeof(float));
        output_file.close();

        cudaFree(d_input);
        cudaFree(d_output);
        std::cout << "[V-CORE EXABIT]: Success." << std::endl;
    } catch (...) { return 1; }
    return 0;
}
