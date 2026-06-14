#
# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).
# 
# SPECIAL RESTRICTION: No use of this code and files (artifacts) is permitted 
# for the training of machine learning models or artificial intelligence 
# without explicit written permission.
# 
# COMMERCIAL CLAUSE: Any enterprise deployment requires a paid commercial license.
# Full license text is available in the LICENSE file in the root directory.
#
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

cmake_minimum_required(VERSION 3.18)

# Инициализируем проект. CXX необходим для основной логики, CUDA — для ядер.
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Настройка архитектуры GPU (75 - Turing, но ставим автоопределение для гибкости)
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

# Поиск CUDA Toolkit (динамический вместо жестких путей)
find_package(CUDAToolkit REQUIRED)

# Вывод информации о найденной версии для логов сборки
message(STATUS "[V-CORE]: CUDA Version: ${CUDAToolkit_VERSION}")
message(STATUS "[V-CORE]: CUDA Compiler: ${CMAKE_CUDA_COMPILER}")

# Подключаем заголовки проекта
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Сборка основного бенчмарка/движка
# Включаем LinearSelector.cu и предполагаемый тестовый файл
add_executable(LinearSelectorBenchmark 
    test_core.cpp 
    LinearSelector.cu
)

# Линковка с библиотеками CUDA
target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)

# Настройки для оптимизации компиляции
set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS OFF
    OUTPUT_NAME "VCORE_v147_STATIC_GPU" # Имя файла для bridge_compressor.py
)

# Установка стандарта C++
set_target_properties(LinearSelectorBenchmark PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

