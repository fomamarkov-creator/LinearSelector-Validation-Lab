cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Отключаем все стандартные поиски библиотек, которые вызывают ошибки
set(CMAKE_CUDA_RESOLVE_DEVICE_SYMBOLS OFF)
set(CMAKE_CUDA_SEPARABLE_COMPILATION OFF)

# Прямая сборка без лишних линковок
add_executable(LinearSelectorBenchmark 
    test_core.cpp 
    LinearSelector.cu
)

# Указываем только базовую библиотеку CUDA
target_link_libraries(LinearSelectorBenchmark PRIVATE cuda)

# Настройка архитектуры
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 80)
endif()
