cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Настройка архитектуры для GitHub (Tesla T4 = 75)
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

include(CheckLanguage)
check_language(CUDA)

add_executable(LinearSelectorBenchmark 
    test_core.cpp 
    LinearSelector.cu
)

if(MSVC)
    target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)
else()
    target_link_libraries(LinearSelectorBenchmark PRIVATE cuda cudart)
endif()

set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS OFF
)
