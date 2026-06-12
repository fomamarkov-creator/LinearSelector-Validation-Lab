# Copyright (C) 2026 Efim Sergeevich Markov (ef.87@mail.ru)
# Licensed under AGPLv3 + AI Training Restriction + Commercial Clause.


import torch
import numpy as np
import os
import subprocess
import shutil
from safetensors.torch import load_file, save_file
from huggingface_hub import hf_hub_download

def run_v144_core(matrix, vector):
    n = vector.shape[0]
    matrix.flatten().astype(np.float32).tofile("temp_weights.bin")
    vector.astype(np.float32).tofile("temp_x.bin")
    result = subprocess.run(["./LinearSelectorBenchmark", str(n), str(n)], capture_output=True)
    if result.returncode != 0:
        return vector
    return np.fromfile("temp_out.bin", dtype=np.float32)

def main():
    print("--- V-CORE: LOW-DISK COMPRESSION START ---")
    repo_id = "Qwen/Qwen2.5-Coder-1.5B"
    
    # Список файлов весов (обычно их 2-3 для такой модели)
    # Мы будем скачивать их по ОДНОМУ
    files_to_download = ["model.safetensors"] # Для 1.5B обычно один основной файл

    for file_name in files_to_download:
        print(f"Downloading {file_name}...")
        file_path = hf_hub_download(repo_id=repo_id, filename=file_name)
        
        weights = load_file(file_path)
        new_weights = {}
        
        for name, tensor in weights.items():
            if "weight" in name and len(tensor.shape) == 2:
                rows, cols = tensor.shape
                if rows <= 2048 and cols <= 2048:
                    h_weight = tensor.to(torch.float32).numpy()
                    v_input = h_weight[0] if rows > 0 else np.zeros(cols)
                    v_res = run_v144_core(h_weight, v_input)
                    
                    res_tensor = torch.from_numpy(v_res)
                    mask = torch.abs(res_tensor) > 0.024
                    res_tensor = res_tensor * mask.float()
                    new_weights[name] = res_tensor.repeat(rows, 1)[:rows, :cols].to(torch.bfloat16)
                else:
                    new_weights[name] = tensor
            else:
                new_weights[name] = tensor
        
        save_file(new_weights, f"v144_{file_name}")
        print(f"Processed and saved: v144_{file_name}")
        
        # УДАЛЯЕМ ОРИГИНАЛ И КЭШ, ЧТОБЫ ОСВОБОДИТЬ МЕСТО
        os.remove(file_path)
        shutil.rmtree(os.path.expanduser("~/.cache/huggingface"), ignore_errors=True)

if __name__ == "__main__":
    main()
