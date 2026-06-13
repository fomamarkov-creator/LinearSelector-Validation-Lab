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

cmake_minimum_required(VERSION 3.18)

# Убираем CUDA из основной строки, чтобы не было ошибки при инициализации
project(LinearSelectorLab LANGUAGES CXX)

# Включаем CUDA отдельно — это позволит нам передать пути из YAML
enable_language(CUDA OPTIONAL)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

# Проверяем, удалось ли включить CUDA
if(CMAKE_CUDA_COMPILER)
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
    message(STATUS "[V-CORE]: CUDA enabled successfully.")
else()
    message(FATAL_ERROR "[V-CORE ERROR]: CUDA compiler not found. Check your YAML paths.")
endif()
