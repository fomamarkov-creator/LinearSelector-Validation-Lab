#!/bin/bash
# Автоматический скрипт валидации для репозитория
sudo apt-get update && apt-get install -y libcudart11.0
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
chmod +x ./LinearSelectorBenchmark-Linux
./LinearSelectorBenchmark-Linux
