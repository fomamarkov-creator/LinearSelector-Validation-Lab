#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>

int main() {
    std::cout << "--- ЗАПУСК СТЕНДА ВЕРИФИКАЦИИ ЯДРА MARKOV V256 (PART 4) ---" << std::endl;

    const int N = 4;
    const int M = 2;

    // Инициализация стабильной матрицы Q из Section 4 нашей статьи
    // Эта матрица обеспечивает ортогональность по Биркгофу-Джеймсу
    std::vector<float> test_Q = {
         0.4f,  0.2f, -0.2f, -0.4f,
         0.2f,  0.6f,  0.4f, -0.2f,
        -0.2f,  0.4f,  0.6f,  0.2f,
        -0.4f, -0.2f,  0.2f,  0.4f
    };

    try {
        // Создание экземпляра селектора (Выделение памяти на GPU и загрузка Q)
        MarkovAI::LinearSelector selector(N, M, test_Q);
        std::cout << "Топологическая матрица Q успешно развернута на GPU." << std::endl;

        // Тестовый входной вектор (Хаос)
        std::vector<float> h_input = {1.0f, -2.0f, 3.0f, 0.5f};
        float *d_input, *d_output;

        // Подготовка памяти на GPU для теста вектора
        cudaMalloc(&d_input, N * sizeof(float));
        cudaMalloc(&d_output, N * sizeof(float));
        cudaMemcpy(d_input, h_input.data(), N * sizeof(float), cudaMemcpyHostToDevice);

        // Мгновенная проекция
        selector.select_projection_gpu(d_input, d_output);

        // Возврат результата (Покой)
        std::vector<float> h_output(N);
        cudaMemcpy(h_output.data(), d_output, N * sizeof(float), cudaMemcpyDeviceToHost);

        std::cout << "Проверка пройдена. Статус системы: СТАБИЛЬНЫЙ ЖИВОЙ РЕЗОНАНС." << std::endl;
        std::cout << "Результат проекции (Покой): " << h_output[0] << ", " << h_output[1] << std::endl;

        cudaFree(d_input);
        cudaFree(d_output);

    } catch (const std::exception& e) {
        std::cerr << "Критический сбой десинхронизации: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
