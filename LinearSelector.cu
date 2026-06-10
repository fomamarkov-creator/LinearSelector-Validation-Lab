#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <stdexcept>

namespace MarkovAI {

// Ядро мгновенного проецирования: V = Q * X
__global__ void markov_selector_kernel(const float* __restrict__ Q, 
                                        const float* __restrict__ X, 
                                        float* __restrict__ V, 
                                        int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (row < n) {
        float sum = 0.0f;
        for (int i = 0; i < n; ++i) {
            sum += Q[row * n + i] * X[i];
        }
        V[row] = sum;
    }
}

void LinearSelector::allocateGPU() {
    size_t size = dim_n * dim_n * sizeof(float);
    cudaError_t err = cudaMalloc(&d_Q, size);
    if (err != cudaSuccess) {
        throw std::runtime_error("CUDA malloc failed for matrix Q.");
    }
    // Копирование топологической матрицы Q на устройство
    cudaMemcpy(d_Q, h_Q.data(), size, cudaMemcpyHostToDevice);
}

void LinearSelector::freeGPU() {
    if (d_Q) {
        cudaFree(d_Q);
        d_Q = nullptr;
    }
}

void LinearSelector::select_projection_gpu(const float* d_input_x, float* d_output_v) {
    // Конфигурация потоков для V144/V256 резонанса
    dim3 threadsPerBlock(1, 256);
    dim3 numBlocks(1, (dim_n + threadsPerBlock.y - 1) / threadsPerBlock.y);

    markov_selector_kernel<<<numBlocks, threadsPerBlock>>>(d_Q, d_input_x, d_output_v, dim_n);
    
    // Синхронизация для обеспечения L-стабильности
    cudaDeviceSynchronize(); 
}

} // namespace MarkovAI
