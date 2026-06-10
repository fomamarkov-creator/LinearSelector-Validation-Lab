cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Настройки стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка архитектуры CUDA (80 - для RTX 30xx/40xx, 75 - для 20xx)
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 80)
endif()

# Включаем максимальную оптимизацию и IPO (LTO) для защиты и скорости
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

# Создаем библиотеку из ядра
add_library(LinearSelector_lib STATIC 
    LinearSelector.cu 
    LinearSelector.hpp
)

# Указываем, что это CUDA-код с отдельной компиляцией
set_target_properties(LinearSelector_lib PROPERTIES 
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)

# Создаем исполняемый файл (наш стенд верификации)
add_executable(LinearSelectorBenchmark test_core.cpp)

# Связываем всё воедино
target_link_libraries(LinearSelectorBenchmark PRIVATE LinearSelector_lib)

# Настройки для Windows (чтобы EXE не требовал лишних DLL)
if(WIN32)
    set_target_properties(LinearSelectorBenchmark PROPERTIES 
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
