#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: LinearSelectorBenchmark <N> <M>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    int M = std::stoi(argv[2]);

    try {
        // Загрузка весов слоя
        std::vector<float> h_Q(N * N);
        std::ifstream input_file("temp_weights.bin", std::ios::binary);
        if (!input_file) throw std::runtime_error("Missing temp_weights.bin");
        input_file.read(reinterpret_cast<char*>(h_Q.data()), N * N * sizeof(float));
        input_file.close();

        MarkovAI::LinearSelector selector(N, M, h_Q);

        // Загрузка вектора активаций
        std::vector<float> h_input(N);
        std::ifstream input_x("temp_x.bin", std::ios::binary);
        input_x.read(reinterpret_cast<char*>(h_input.data()), N * sizeof(float));
        input_x.close();

        float *d_input, *d_output;
        cudaMalloc(&d_input, N * sizeof(float));
        cudaMalloc(&d_output, N * sizeof(float));
        cudaMemcpy(d_input, h_input.data(), N * sizeof(float), cudaMemcpyHostToDevice);

        // Резонансная проекция V144
        selector.select_projection_gpu(d_input, d_output);

        std::vector<float> h_output(N);
        cudaMemcpy(h_output.data(), d_output, N * sizeof(float), cudaMemcpyDeviceToHost);

        // Сохранение для Python
        std::ofstream output_file("temp_out.bin", std::ios::binary);
        output_file.write(reinterpret_cast<char*>(h_output.data()), N * sizeof(float));
        output_file.close();

        cudaFree(d_input);
        cudaFree(d_output);

    } catch (const std::exception& e) {
        std::cerr << "V-CORE ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
