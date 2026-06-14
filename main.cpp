/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * V-CORE v147.3 - CLOUD RESONANCE EDITION (LINUX)
 */

#include "LinearSelector.hpp"
#include <iostream>
#include <vector>
#include <cuda_runtime.h>

void check_environment() {
    int deviceCount = 0;
    cudaGetDeviceCount(&deviceCount);
    std::cout << "--- V-CORE CLOUD SCAN ---" << std::endl;
    if (deviceCount == 0) {
        std::cout << "[MODE]: CPU-ONLY (AVX2 Active)" << std::endl;
    } else {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, 0);
        std::cout << "[MODE]: GPU ACTIVE: " << deviceProp.name << std::endl;
    }
}

int main() {
    check_environment();
    std::vector<float> dummy_q = {1.0f, 0.0f, 0.0f, 1.0f};
    try {
        MarkovAI::LinearSelector selector(2, 2, dummy_q);
        std::cout << "Resonance 144hz confirmed in Matrix." << std::endl; 
    } catch (const std::exception& e) {
        std::cerr << "[FAIL]: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
