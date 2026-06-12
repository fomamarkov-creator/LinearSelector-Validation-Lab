#ifndef LINEAR_SELECTOR_HPP
#define LINEAR_SELECTOR_HPP

#include <vector>
#include <stdexcept>

namespace MarkovAI {

class LinearSelector {
private:
    int dim_n;             // Размерность пространства
    int dim_m;             // Размерность подпространства
    std::vector<float> h_Q; // Матрица Q на CPU
    float* d_Q;            // Матрица Q на GPU

public:
    // Инициализация с проверкой размерности N x N
    LinearSelector(int n, int m, const std::vector<float>& matrix_Q) 
        : dim_n(n), dim_m(m), d_Q(nullptr) {
        if (matrix_Q.size() != static_cast<size_t>(n * n)) {
            throw std::invalid_argument("Ошибка: Размерность матрицы Q должна быть N x N.");
        }
        h_Q = matrix_Q;
        allocateGPU();
    }

    ~LinearSelector() {
        freeGPU();
    }

    // Запрет копирования (защита от утечек памяти CUDA)
    LinearSelector(const LinearSelector&) = delete;
    LinearSelector& operator=(const LinearSelector&) = delete;

    // Метод запуска вычислений на GPU
    void select_projection_gpu(const float* d_input_x, float* d_output_v);

private:
    void allocateGPU();
    void freeGPU();
};

} // namespace MarkovAI

#endif
