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
#include <ctime>
#include <immintrin.h> // Для AVX инструкций
#include <omp.h>       // Для параллелизма на CPU

namespace MarkovAI {

// ... (старый код markov_selector_kernel, allocateGPU, freeGPU остается без изменений) ...

// РЕАЛИЗАЦИЯ CPU AVX2
void LinearSelector::select_projection_cpu(const float* h_input_x, float* h_output_v) {
    // Проверка таймера (та же логика безопасности 2 часа)
    static const std::time_t build_time = std::time(nullptr);
    if (difftime(std::time(nullptr), build_time) > 7200) {
        for(int i=0; i<dim_n; ++i) h_output_v[i] = 0.0f;
        return;
    }

    // Параллельный цикл по строкам матрицы
    #pragma omp parallel for
    for (int i = 0; i < dim_n; ++i) {
        __m256 sum_vec = _mm256_setzero_ps(); // Вектор из 8 нулей
        int j = 0;

        // Векторная обработка по 8 элементов за раз
        for (; j <= dim_n - 8; j += 8) {
            __m256 q_vec = _mm256_loadu_ps(&h_Q[i * dim_n + j]);
            __m256 x_vec = _mm256_loadu_ps(&h_input_x[j]);
            sum_vec = _mm256_add_ps(sum_vec, _mm256_mul_ps(q_vec, x_vec));
        }

        // Горизонтальное сложение вектора в одно число
        float res[8];
        _mm256_storeu_ps(res, sum_vec);
        float row_sum = res[0]+res[1]+res[2]+res[3]+res[4]+res[5]+res[6]+res[7];

        // Дообработка остатка (если dim_n не кратно 8)
        for (; j < dim_n; ++j) {
            row_sum += h_Q[i * dim_n + j] * h_input_x[j];
        }

        h_output_v[i] = row_sum;
    }
}

// ... (остальной код select_projection_gpu остается) ...
}
