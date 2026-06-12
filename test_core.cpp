#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

// --- БЛОК V-CORE: МАТЕМАТИЧЕСКОЕ СЖАТИЕ ---
struct VNode {
    float base;
    int exp;
    float offset;
};

// Реализация леммы universal_reduction: (3*x + 1)/4 < x
float apply_vcore_compression(float value) {
    if (std::abs(value) < 1e-9f) return 0.0f;

    int exp;
    float frac = std::frexp(value, &exp);
    
    // V-CORE LOGIC: Если экспонента позволяет (exp > 2), 
    // мы схлопываем дробную часть для уменьшения энтропии
    if (exp > 2) {
        // Имитация спуска: уменьшаем "массу" числа, сохраняя его резонанс
        frac = (3.0f * frac) / 4.0f; 
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
        input_file.read(reinterpret_cast<char*>(h_Q.data()), N * N * sizeof(float));
        input_file.close();

        // Перед загрузкой в GPU применяем V-CORE сжатие к матрице весов
        for (float& val : h_Q) {
            val = apply_vcore_compression(val);
        }

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

        // Финальное сжатие выходного вектора перед сохранением
        for (float& val : h_output) {
            val = apply_vcore_compression(val);
        }

        std::ofstream output_file("temp_out.bin", std::ios::binary);
        output_file.write(reinterpret_cast<char*>(h_output.data()), N * sizeof(float));
        output_file.close();

        cudaFree(d_input);
        cudaFree(d_output);
        std::cout << "[V-CORE]: Сжатие слоя завершено успешно." << std::endl;

    } catch (...) {
        return 1;
    }
    return 0;
}
