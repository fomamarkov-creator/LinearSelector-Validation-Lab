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

# Инициализируем только CXX. Это КРИТИЧНО, чтобы избежать ошибки 0x2 в начале.
project(LinearSelectorLab LANGUAGES CXX)

# Вручную указываем пути к CUDA, так как автоматика на GitHub Actions ломается
set(CUDA_PATH "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8")
include_directories("${CUDA_PATH}/include")
link_directories("${CUDA_PATH}/lib/x64")

# Пытаемся включить CUDA. Если она не подхватится автоматом, мы слинкуем её вручную ниже.
enable_language(CUDA OPTIONAL)

if(CMAKE_CUDA_COMPILER)
    message(STATUS "[V-CORE]: CUDA Compiler found: ${CMAKE_CUDA_COMPILER}")
    set(CMAKE_CUDA_ARCHITECTURES 75)
    
    add_executable(LinearSelectorBenchmark 
        test_core.cpp 
        LinearSelector.cu
    )
    
    target_link_libraries(LinearSelectorBenchmark PRIVATE CUDA::cudart)
else()
    # ЭКСТРЕМАЛЬНЫЙ ВАРИАНТ: Если enable_language всё равно не сработал
    message(WARNING "[V-CORE]: CUDA not enabled via CMake, attempting manual link...")
    add_executable(LinearSelectorBenchmark test_core.cpp)
    # Линкуем библиотеку CUDA напрямую по пути
    target_link_libraries(LinearSelectorBenchmark PRIVATE "${CUDA_PATH}/lib/x64/cudart.lib")
endif()

set_target_properties(LinearSelectorBenchmark PROPERTIES
    CUDA_SEPARABLE_COMPILATION OFF
    CUDA_RESOLVE_DEVICE_SYMBOLS OFF
)
