import torch
import numpy as np
import os
import subprocess
from safetensors.torch import load_file, save_file
from huggingface_hub import snapshot_download

def run_v144_core(matrix, vector):
    """Передает данные в ваше CUDA-ядро через бинарные файлы"""
    n = vector.shape[0]
    # Сохраняем во временные файлы
    matrix.astype(np.float32).tofile("temp_weights.bin")
    vector.astype(np.float32).tofile("temp_x.bin")
    
    # Запускаем ваш скомпилированный бинарник
    # Предполагаем, что он скомпилирован как 'LinearSelectorApp'
    result = subprocess.run(["./LinearSelectorApp", str(n), str(n)], capture_output=True)
    
    if result.returncode != 0:
        print(f"Ошибка ядра: {result.stderr.decode()}")
        return vector # В случае ошибки возвращаем оригинал
    
    # Читаем результат
    return np.fromfile("temp_out.bin", dtype=np.float32)

def main():
    print("--- MARKOV V-CORE: QWEN 2.5 ADAPTATION START ---")
    
    # 1. Скачиваем модель (только нужные веса)
    print("Скачивание модели Qwen 2.5-Coder-1.5B...")
    model_dir = snapshot_download(
        repo_id="Qwen/Qwen2.5-Coder-1.5B", 
        allow_patterns=["*.safetensors"]
    )
    
    # Находим файл с весами
    st_files = [f for f in os.listdir(model_dir) if f.endswith(".safetensors")]
    
    for st_file in st_files:
        full_path = os.path.join(model_dir, st_file)
        weights = load_file(full_path)
        new_weights = {}
        
        for name, tensor in weights.items():
            # Оптимизируем только матрицы весов (Linear layers)
            if "weight" in name and len(tensor.shape) == 2:
                rows, cols = tensor.shape
                # Для теста берем квадратную подматрицу или обрабатываем по частям
                # В данной версии берем векторный срез для проекции
                print(f"Обработка слоя: {name} [{rows}x{cols}]")
                
                # Пример: прогоняем через резонанс V144
                h_weight = tensor.to(torch.float32).numpy()
                # Мы берем первую строку как тестовый вектор для проекции
                v_input = h_weight[0] 
                
                v_resonant = run_v144_core(h_weight[:min(rows,cols), :min(rows,cols)], v_input)
                
                # Временно возвращаем результат в тензор (в будущем тут будет сжатие)
                # Сейчас мы просто подтверждаем прохождение данных через ядро
                new_weights[name] = tensor 
            else:
                new_weights[name] = tensor
        
        # Сохраняем результат
        save_name = f"v144_{st_file}"
        save_file(new_weights, save_name)
        print(f"Файл {save_name} подготовлен для Asus X541N.")

if __name__ == "__main__":
    main()
