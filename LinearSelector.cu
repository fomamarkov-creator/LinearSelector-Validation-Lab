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
#include <stdexcept>
#include <ctime>
#include <string>

namespace MarkovAI {

// Ядро мгновенного проецирования с проверкой активации
__global__ void markov_selector_kernel(const float* __restrict__ Q, 
                                        const float* __restrict__ X, 
                                        float* __restrict__ V, 
                                        int n,
                                        bool active) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < n) {
        if (!active) {
            V[row] = 0.0f; // Деактивация: алгоритм выдает "нулевой покой"
            return;
        }
        
        float sum = 0.0f;
        for (int i = 0; i < n; ++i) {
            sum += Q[row * n + i] * X[i];
        }
        V[row] = sum;
    }
}

void LinearSelector::allocateGPU() {
    size_t size = dim_n * dim_n * sizeof(float);
    cudaMalloc(&d_Q, size);
    cudaMemcpy(d_Q, h_Q.data(), size, cudaMemcpyHostToDevice);
}

void LinearSelector::freeGPU() {
    if (d_Q) {
        cudaFree(d_Q);
        d_Q = nullptr;
    }
}

void LinearSelector::select_projection_gpu(const float* d_input_x, float* d_output_v) {
    // --- СИСТЕМА ЭКСПРЕСС-ДЕАКТИВАЦИИ (ОГРАНИЧЕНИЕ 2 ЧАСА) ---
    static const std::time_t build_time = std::time(nullptr); // Фиксация времени запуска
    std::time_t now = std::time(nullptr);
    
    // 7200 секунд = 2 часа (согласно директиве v146 LIGO)
    bool is_active = (difftime(now, build_time) < 7200);
    
    if (!is_active) {
        std::cerr << "[SECURITY ALERT]: V-CORE v147 LIGO Demo license expired (2-hour limit)." << std::endl;
        std::cerr << "[STATUS]: Transition to STATIC mode. All outputs set to ZERO." << std::endl;
        std::cerr << "[CONTACT]: For full access contact ef.87@mail.ru" << std::endl;
    }

    dim3 threadsPerBlock(1, 256);
    dim3 numBlocks(1, (dim_n + threadsPerBlock.y - 1) / threadsPerBlock.y);

    markov_selector_kernel<<<numBlocks, threadsPerBlock>>>(d_Q, d_input_x, d_output_v, dim_n, is_active);
    
    cudaDeviceSynchronize(); 
}

} // namespace MarkovAI
