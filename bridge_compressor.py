import torch
import numpy as np
import os
import subprocess
from safetensors.torch import load_file, save_file
from huggingface_hub import snapshot_download

def run_v144_core(matrix, vector):
    n = vector.shape[0]
    matrix.flatten().astype(np.float32).tofile("temp_weights.bin")
    vector.astype(np.float32).tofile("temp_x.bin")
    
    # Вызов вашего скомпилированного ядра
    result = subprocess.run(["./LinearSelectorBenchmark", str(n), str(n)], capture_output=True)
    
    if result.returncode != 0:
        return vector
    return np.fromfile("temp_out.bin", dtype=np.float32)

def main():
    print("--- V-CORE: STARTING QWEN 2.5 OPTIMIZATION ---")
    model_dir = snapshot_download(repo_id="Qwen/Qwen2.5-Coder-1.5B", allow_patterns=["*.safetensors"])
    
    for st_file in [f for f in os.listdir(model_dir) if f.endswith(".safetensors")]:
        weights = load_file(os.path.join(model_dir, st_file))
        new_weights = {}
        for name, tensor in weights.items():
            if "weight" in name and len(tensor.shape) == 2:
                # Ограничим размер для теста, чтобы влезть в 15 минут демо-лицензии
                rows, cols = tensor.shape
                if rows == cols and rows <= 1536: 
                    print(f"Optimizing: {name}")
                    h_weight = tensor.to(torch.float32).numpy()
                    # Прогонка через резонанс
                    res = run_v144_core(h_weight, h_weight[0])
                    new_weights[name] = torch.from_numpy(res).repeat(rows, 1).to(torch.bfloat16)
                else:
                    new_weights[name] = tensor
            else:
                new_weights[name] = tensor
        save_file(new_weights, f"v144_{st_file}")

if __name__ == "__main__":
    main()
