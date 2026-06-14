# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

cmake_minimum_required(VERSION 3.18)

# Инициализируем CXX и CUDA вместе, но с проверкой
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Настройка архитектуры
set(CMAKE_CUDA_ARCHITECTURES 75)

# Явно указываем, что нам нужно скомпилировать и связать эти файлы
add_executable(LinearSelectorBenchmark 
    main.cpp 
    LinearSelector.cu
    LinearSelector.hpp
)

# Указываем, что LinearSelector.cu должен компилироваться компилятором CUDA
set_source_files_properties(LinearSelector.cu PROPERTIES LANGUAGE CUDA)

target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)

set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    OUTPUT_NAME "VCORE_v147_STATIC_GPU"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
