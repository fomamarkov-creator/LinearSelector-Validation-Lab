#include "LinearSelector.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

/**
 * MARKOV CORE V144: INDUSTRIAL ADAPTER
 * Этот модуль связывает веса нейросети с вашим CUDA-ядром.
 */

int main(int argc, char* argv[]) {
    // Проверка аргументов: размер матрицы N и размер вектора M
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <M>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]); // Размерность слоя (например, 1536)
    int M = std::stoi(argv[2]); // Проекционная база

    try {
        // 1. Загрузка весов из временного бинарного файла (подготовленного Python)
        std::vector<float> h_Q(N * N);
        std::ifstream input_file("temp_weights.bin", std::ios::binary);
        if (!input_file) throw std::runtime_error("Cannot open temp_weights.bin");
        input_file.read(reinterpret_cast<char*>(h_Q.data()), N * N * sizeof(float));
        input_file.close();

        // 2. Инициализация вашего ядра LinearSelector под размер слоя N
        MarkovAI::LinearSelector selector(N, M, h_Q);

        // 3. Подготовка входных векторов (активаций или весов)
        std::vector<float> h_input(N);
        std::ifstream input_x("temp_x.bin", std::ios::binary);
        input_x.read(reinterpret_cast<char*>(h_input.data()), N * sizeof(float));
        input_x.close();

        float *d_input, *d_output;
        cudaMalloc(&d_input, N * sizeof(float));
        cudaMalloc(&d_output, N * sizeof(float));
        cudaMemcpy(d_input, h_input.data(), N * sizeof(float), cudaMemcpyHostToDevice);

        // 4. Запуск вашего резонансного проецирования
        selector.select_projection_gpu(d_input, d_output);

        // 5. Сохранение результата обратно в бинарный файл для Python
        std::vector<float> h_output(N);
        cudaMemcpy(h_output.data(), d_output, N * sizeof(float), cudaMemcpyDeviceToHost);

        std::ofstream output_file("temp_out.bin", std::ios::binary);
        output_file.write(reinterpret_cast<char*>(h_output.data()), N * sizeof(float));
        output_file.close();

        std::cout << "[V144 CORE]: Processing layer N=" << N << " completed." << std::endl;

        cudaFree(d_input);
        cudaFree(d_output);

    } catch (const std::exception& e) {
        std::cerr << "[CRITICAL]: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
