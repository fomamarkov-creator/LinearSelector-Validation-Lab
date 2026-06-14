# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

cmake_minimum_required(VERSION 3.18)

# Инициализируем только CXX, чтобы избежать ошибки определения CUDA компилятора
project(LinearSelectorLab LANGUAGES CXX)

# Ручное указание путей к CUDA (соответствует установке в GitHub Actions)
set(CUDA_PATH "$ENV{CUDA_PATH}")
if(NOT CUDA_PATH)
    set(CUDA_PATH "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8")
endif()

message(STATUS "[V-CORE]: Using CUDA Path: ${CUDA_PATH}")

include_directories("${CUDA_PATH}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Собираем движок из main.cpp и LinearSelector.cu (CMake обработает .cu как C++ файлы)
# Но мы скомпилируем их через NVCC вручную, если этот метод не сработает.
add_executable(LinearSelectorBenchmark 
    main.cpp 
    LinearSelector.cu
)

# Прямая линковка библиотек рантайма CUDA
target_link_libraries(LinearSelectorBenchmark PRIVATE "${CUDA_PATH}/lib/x64/cudart.lib")

set_target_properties(LinearSelectorBenchmark PROPERTIES
    OUTPUT_NAME "VCORE_v147_STATIC_GPU"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
