# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

cmake_minimum_required(VERSION 3.18)

# Инициализируем ТОЛЬКО CXX. Это исключит ошибку поиска компилятора CUDA.
project(LinearSelectorLab LANGUAGES CXX)

# Автоматическое определение путей для Google Colab (Linux)
set(CUDA_PATH "/usr/local/cuda")
include_directories("${CUDA_PATH}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Добавляем файлы
add_executable(LinearSelectorBenchmark 
    main.cpp 
    LinearSelector.cu
    LinearSelector.hpp
)

# Линкуем Linux-версию библиотеки времени выполнения CUDA напрямую
target_link_libraries(LinearSelectorBenchmark PRIVATE "${CUDA_PATH}/lib64/libcudart.so")

set_target_properties(LinearSelectorBenchmark PROPERTIES
    OUTPUT_NAME "vcore_engine_linux"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
