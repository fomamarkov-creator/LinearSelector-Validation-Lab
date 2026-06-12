#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <cstdint>

/**
 * MARKOV V-CORE: TERABIT SYNCHRONIZATION ENGINE
 * Интеграция леммы universal_reduction и терабитных сдвигов.
 */

// Логика Терабитной синхронизации: 3 * 2^(n-1) + 2
// Это мгновенное схлопывание экспоненциального веса
uint64_t vcore_terabit_sync(int exp) {
    if (exp <= 0) return 1;
    // Ограничиваем сдвиг 62 битами для безопасности uint64
    uint64_t n = static_cast<uint64_t>(std::abs(exp)) % 62;
    if (n == 0) n = 1;
    return (3ULL << (n - 1)) + 2;
}

// Применение универсальной леммы сжатия к весу нейрона
float apply_vcore_compression(float value) {
    if (std::abs(value) < 1e-9f) return 0.0f;

    int exp;
    float frac = std::frexp(value, &exp);
    
    // 1. Применяем терабитную синхронизацию экспоненты
    uint64_t sync_factor = vcore_terabit_sync(exp);
    
    // 2. Реализация леммы: (3*x + 1)/4 < x
    // Мы используем sync_factor как модификатор "массы" числа
    if (exp > 2) {
        float mass_reduction = static_cast<float>(sync_factor % 1024) / 1024.0f;
        frac = (3.0f * frac * mass_reduction) / 4.0f;
    }
    
    // 3. Обратная сборка числа после квантового спуска
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

        // ФАЗА 1: Терабитная синхронизация матрицы весов
        for (float& val : h_Q) {
            val = apply_vcore_compression(val);
        }

        MarkovAI::LinearSelector selector(N, M, h_Q);

        std::vector<float> h_input(N);
        std::ifstream input_x("temp_x.bin", std::ios::binary);
        if (!input_x) return 1;
        input_x.read(reinterpret_cast<char*>(h_input.data()), N * sizeof(float));
        input_x.close();

        float *d_input, *d_output;
        cudaMalloc(&d_input, N * sizeof(float));
        cudaMalloc(&d_output, N * sizeof(float));
        cudaMemcpy(d_input, h_input.data(), N * sizeof(float), cudaMemcpyHostToDevice);

        // ФАЗА 2: Проекция через CUDA-ядро V144
        selector.select_projection_gpu(d_input, d_output);

        std::vector<float> h_output(N);
        cudaMemcpy(h_output.data(), d_output, N * sizeof(float), cudaMemcpyDeviceToHost);

        // ФАЗА 3: Финальное схлопывание вектора активаций
        for (float& val : h_output) {
            val = apply_vcore_compression(val);
        }

        std::ofstream output_file("temp_out.bin", std::ios::binary);
        output_file.write(reinterpret_cast<char*>(h_output.data()), N * sizeof(float));
        output_file.close();

        cudaFree(d_input);
        cudaFree(d_output);
        
        std::cout << "[V-CORE]: Terabit Sync Completed. N=" << N << std::endl;

    } catch (...) {
        return 1;
    }
    return 0;
}
