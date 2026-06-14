/*
 * Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
 * Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).
 * 
 * SPECIAL RESTRICTION: No use of this code and files (artifacts) is permitted 
 * for the training of machine learning models or artificial intelligence 
 * without explicit written permission.
 */

#ifndef LINEAR_SELECTOR_HPP
#define LINEAR_SELECTOR_HPP

#include <vector>
#include <stdexcept>

namespace MarkovAI {

class LinearSelector {
private:
    int dim_n;             
    int dim_m;             
    std::vector<float> h_Q; 
    float* d_Q;            

public:
    LinearSelector(int n, int m, const std::vector<float>& matrix_Q);
    ~LinearSelector();

    // Запрет копирования
    LinearSelector(const LinearSelector&) = delete;
    LinearSelector& operator=(const LinearSelector&) = delete;

    // МЕТОД 1: Вычисления на GPU (CUDA)
    void select_projection_gpu(const float* d_input_x, float* d_output_v);

    // МЕТОД 2: Вычисления на CPU (AVX2/SIMD) - НОВОЕ
    // Принимает обычные указатели на оперативную память
    void select_projection_cpu(const float* h_input_x, float* h_output_v);

private:
    void allocateGPU();
    void freeGPU();
};

} // namespace MarkovAI

#endif
