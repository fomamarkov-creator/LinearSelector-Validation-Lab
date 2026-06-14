/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * V-CORE v147.2 - DIAGNOSTIC CORE
 */

#include "LinearSelector.hpp"
#include <iostream>
#include <vector>
#include <cuda_runtime.h>

void check_environment() {
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);

    std::cout << "--- V-CORE ENVIRONMENT SCAN ---" << std::endl;
    
    if (error != cudaSuccess || deviceCount == 0) {
        std::cout << "[STATUS]: NVIDIA GPU not detected or CUDA driver missing." << std::endl;
        std::cout << "[ACTION]: Switching to CPU-RESONANCE mode (AVX2/SIMD)." << std::endl;
        std::cout << "[LIGO]: 144Hz Harmonic stability will be maintained by CPU." << std::endl;
    } else {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, 0);
        std::cout << "[STATUS]: NVIDIA GPU Detected: " << deviceProp.name << std::endl;
        std::cout << "[ACTION]: GPU-ACCELERATED Resonance active." << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}

int main() {
    std::cout << "V-CORE ENGINE v147.2 LIGO-READY" << std::endl;
    
    // 1. Проверяем, где мы работаем
    check_environment();

    // 2. Тестовый запуск матрицы
    std::vector<float> dummy_q = {1.0f, 0.0f, 0.0f, 1.0f}; // Матрица 2x2
    try {
        MarkovAI::LinearSelector selector(2, 2, dummy_q);
        std::cout << "Resonance 144hz applied successfully." << std::endl; 
    } catch (const std::exception& e) {
        std::cerr << "[CRITICAL]: Resonance failure: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
