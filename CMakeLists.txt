# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

cmake_minimum_required(VERSION 3.18)

# Инициализируем ТОЛЬКО CXX. Это исключит ошибку поиска компилятора CUDA.
project(LinearSelectorLab LANGUAGES CXX)

# Ручное указание путей (совместимо с GitHub и твоим ПК)
set(CUDA_PATH "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8")
include_directories("${CUDA_PATH}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Добавляем файлы
add_executable(LinearSelectorBenchmark 
    main.cpp 
    LinearSelector.cu
    LinearSelector.hpp
)

# Линкуем библиотеку напрямую. Если GPU нет, линковщик пропустит CUDA-символы.
target_link_libraries(LinearSelectorBenchmark PRIVATE "${CUDA_PATH}/lib/x64/cudart.lib")

set_target_properties(LinearSelectorBenchmark PROPERTIES
    OUTPUT_NAME "VCORE_v147_STATIC_GPU"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
