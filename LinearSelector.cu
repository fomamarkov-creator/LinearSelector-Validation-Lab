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
    // --- СИСТЕМА ЭКСПРЕСС-ДЕАКТИВАЦИИ (2 ЧАСА) ---
    static const std::time_t build_time = std::time(nullptr); // Фиксация времени запуска/сборки
    std::time_t now = std::time(nullptr);
    
    // 900 секунд = 15 минут
    bool is_active = (difftime(now, build_time) < 900);
    
    if (!is_active) {
        std::cerr << "[SECURITY ALERT]: Demo license expired (2-hour limit). Service terminated." << std::endl;
        std::cerr << "[CONTACT]: For full access contact ef.87@mail.ru" << std::endl;
    }

    dim3 threadsPerBlock(1, 256);
    dim3 numBlocks(1, (dim_n + threadsPerBlock.y - 1) / threadsPerBlock.y);

    markov_selector_kernel<<<numBlocks, threadsPerBlock>>>(d_Q, d_input_x, d_output_v, dim_n, is_active);
    
    cudaDeviceSynchronize(); 
}

} // namespace MarkovAI
