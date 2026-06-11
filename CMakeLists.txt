cmake_minimum_required(VERSION 3.18)
project(LinearSelectorLab LANGUAGES CXX CUDA)

# Настройка архитектуры (Ampere/A100 по умолчанию)
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 80)
endif()

# Позволяет CMake автоматически найти все нужные пути к CUDA
include(CheckLanguage)
check_language(CUDA)

add_executable(LinearSelectorBenchmark 
    test_core.cpp 
    LinearSelector.cu
)

# Для Windows (MSVC) нужны особые настройки линковки
if(MSVC)
    # На Windows линкуем через встроенный механизм CMake для CUDA
    target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)
else()
    # На Linux оставляем твой вариант или тоже используем стандарт
    target_link_libraries(LinearSelectorBenchmark PRIVATE cuda cudart)
endif()

# Убираем лишние зависимости, как ты и хотел
set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS OFF
)
