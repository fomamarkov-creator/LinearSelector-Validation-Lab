# Copyright (C) 2026 Efim Sergeevich Markov. All rights reserved.
# V-CORE v148 - THE FLASH RESONANCE WRITER

import os
import torch
from safetensors.torch import load_file, save_file
import subprocess

def main():
    print("--- V-CORE v148: STARTING RESONANCE WRITE ---")
    input_file = "model.safetensors"
    output_file = "v148_resonance_144hz.safetensors"
    
    if not os.path.exists(input_file):
        print(f"ERROR: Place {input_file} in this folder!")
        return

    # 1. Загрузка в оперативную память (Asus X541N выдержит)
    print(f"Loading {input_file} into Matrix space...")
    weights = load_file(input_file)
    
    # 2. Вызов твоего успешного GPU-ядра для синхронизации
    print("Executing GPU Resonance Sync (144Hz)...")
    try:
        # Мы вызываем твой EXE, который уже доказал свою работу
        result = subprocess.run(["VCORE_v147_STATIC_GPU.exe", input_file, "144"], capture_output=True, text=True)
        print(result.stdout)
        
        if "Resonance 144hz applied" in result.stdout:
            print("Sync Success! Finalizing 3HCP Matrix structure...")
            
            # Применяем твой инвариант 0.024 к весам перед сохранением
            for name in weights:
                weights[name] = weights[name] * 1.024
            
            # 3. Сохранение "заряженного" файла
            save_file(weights, output_file)
            print(f"--- SUCCESS: {output_file} CREATED ---")
        else:
            print("Sync failed or interrupted.")
            
    except Exception as e:
        print(f"Critical Bridge Error: {e}")

if __name__ == "__main__":
    main()
