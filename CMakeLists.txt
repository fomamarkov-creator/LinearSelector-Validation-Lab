cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явный поиск CUDA
find_package(CUDAToolkit REQUIRED)

# Отключаем раздельную компиляцию для стабильности на Ubuntu
set(CMAKE_CUDA_SEPARABLE_COMPILATION OFF)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 80)
endif()

# Создаем исполняемый файл напрямую из всех исходников
add_executable(LinearSelectorBenchmark 
    test_core.cpp 
    LinearSelector.cu 
    LinearSelector.hpp
)

# Используем найденные библиотеки CUDAToolkit
target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)

# Настройка для корректной линковки на Linux
set_target_properties(LinearSelectorBenchmark PROPERTIES 
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
)
