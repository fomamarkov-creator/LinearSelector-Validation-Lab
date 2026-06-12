import torch
import numpy as np
import os
import subprocess
from safetensors.torch import load_file, save_file
from huggingface_hub import snapshot_download

def run_v144_core(matrix, vector):
    """Вызов вашего CUDA-ядра с V-CORE сжатием"""
    n = vector.shape[0]
    # Подготовка бинарников для C++
    matrix.flatten().astype(np.float32).tofile("temp_weights.bin")
    vector.astype(np.float32).tofile("temp_x.bin")
    
    # Запуск LinearSelectorBenchmark
    result = subprocess.run(["./LinearSelectorBenchmark", str(n), str(n)], capture_output=True)
    
    if result.returncode != 0:
        print(f"Core Error: {result.stderr.decode()}")
        return vector
        
    return np.fromfile("temp_out.bin", dtype=np.float32)

def main():
    print("--- V-CORE: REAL COMPRESSION MODE ACTIVE ---")
    # Скачиваем только нужную часть Qwen (1.5B)
    model_dir = snapshot_download(repo_id="Qwen/Qwen2.5-Coder-1.5B", allow_patterns=["*.safetensors"])
    
    for st_file in [f for f in os.listdir(model_dir) if f.endswith(".safetensors")]:
        print(f"Processing file: {st_file}")
        weights = load_file(os.path.join(model_dir, st_file))
        new_weights = {}
        
        for name, tensor in weights.items():
            # Сжимаем только тяжелые слои внимания и MLP
            if "weight" in name and len(tensor.shape) == 2:
                rows, cols = tensor.shape
                # Обрабатываем квадратные блоки, подходящие под вашу решетку
                if rows <= 2048 and cols <= 2048:
                    print(f"  V-CORE Squeezing: {name} ({rows}x{cols})")
                    h_weight = tensor.to(torch.float32).numpy()
                    
                    # Прогоняем через резонанс V144 (берём диагональ как вектор активации)
                    v_input = np.diag(h_weight) if rows == cols else h_weight[0]
                    v_res = run_v144_core(h_weight, v_input)
                    
                    # ПРИМЕНЕНИЕ РЕЗОНАНСНОГО ФИЛЬТРА: 
                    # Обнуляем всё, что не попало в гармонику (ниже порога)
                    res_tensor = torch.from_numpy(v_res)
                    mask = torch.abs(res_tensor) > 0.024 # Ваш порог десинхронизации!
                    res_tensor = res_tensor * mask.float()
                    
                    # Восстанавливаем структуру (упрощенная проекция)
                    new_weights[name] = res_tensor.repeat(rows, 1)[:rows, :cols].to(torch.bfloat16)
                else:
                    new_weights[name] = tensor
            else:
                new_weights[name] = tensor
                
        save_file(new_weights, f"v144_{st_file}")
        print(f"Saved: v144_{st_file}")

if __name__ == "__main__":
    main()
