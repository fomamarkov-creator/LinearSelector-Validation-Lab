# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

find_package(CUDAToolkit REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ЗАМЕНА: Теперь используем main.cpp вместо удаленного test_core.cpp
add_executable(LinearSelectorBenchmark 
    main.cpp 
    LinearSelector.cu
)

target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)

set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS OFF
    OUTPUT_NAME "VCORE_v147_STATIC_GPU"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
