# Copyright (C) 2026 Efim Sergeevich Markov. All rights reserved.
# MARKOV SHIELD GUI v149.1 - OPERATIONAL INTERFACE

import customtkinter as ctk
from tkinter import filedialog
import threading
import sys
import os
import bridge_compressor # Импортируем нашу исправленную логику

class MarkovShieldGUI(ctk.CTk):
    def __init__(self):
        super().__init__()

        # Цветовая схема v146 LIGO
        self.BORDER_COLOR = "#7898d0"
        self.INNER_BLUE = "#3c6cb4"
        self.BTN_COLOR = "#5c8cd4"

        self.title("MARKOV SHIELD V-CORE v149.1 [LIGO-READY]")
        self.geometry("1000x700")
        self.configure(fg_color=self.BORDER_COLOR)

        # ВЕРХНЕЕ ОКНО (Логи и Ответы)
        self.output_frame = ctk.CTkFrame(self, fg_color=self.INNER_BLUE, corner_radius=15, border_width=2, border_color="#FFFFFF")
        self.output_frame.pack(pady=(25, 10), padx=25, fill="both", expand=True)

        self.output_text = ctk.CTkTextbox(self.output_frame, fg_color="transparent", text_color="white", font=("Verdana", 14))
        self.output_text.pack(padx=10, pady=10, fill="both", expand=True)
        self.log_message("--- V-CORE v149.1: СИСТЕМА ИНИЦИАЛИЗИРОВАНА ---")

        # НИЖНЕЕ ОКНО (Управление)
        self.input_frame = ctk.CTkFrame(self, height=160, fg_color=self.INNER_BLUE, corner_radius=15, border_width=2, border_color="#FFFFFF")
        self.input_frame.pack(pady=(0, 25), padx=25, fill="x")
        self.input_frame.pack_propagate(False)

        self.input_entry = ctk.CTkEntry(self.input_frame, placeholder_text="Путь к модели...", 
                                        fg_color="transparent", border_width=0, text_color="white", font=("Verdana", 14))
        self.input_entry.pack(side="left", padx=20, fill="both", expand=True)

        self.btn_container = ctk.CTkFrame(self.input_frame, fg_color="transparent")
        self.btn_container.pack(side="right", padx=15, pady=15, anchor="s")

        btn_params = {
            "width": 120, "height": 38, "fg_color": self.BTN_COLOR, 
            "corner_radius": 8, "border_width": 2, "border_color": "#FFFFFF", 
            "text_color": "white", "font": ("Verdana", 10, "bold")
        }
        
        self.copy_btn = ctk.CTkButton(self.btn_container, text="COPY", command=self.copy_logs, **btn_params)
        self.copy_btn.pack(side="left", padx=5)

        self.load_btn = ctk.CTkButton(self.btn_container, text="LOAD", command=self.load_file, **btn_params)
        self.load_btn.pack(side="left", padx=5)

        self.send_btn = ctk.CTkButton(self.btn_container, text="SEND", command=self.start_resonance, **btn_params)
        self.send_btn.pack(side="left", padx=5)

    def log_message(self, message):
        self.output_text.insert("end", f"{message}\n")
        self.output_text.see("end")

    def load_file(self):
        filename = filedialog.askopenfilename(filetypes=[("Safetensors", "*.safetensors")])
        if filename:
            self.input_entry.delete(0, "end")
            self.input_entry.insert(0, filename)
            self.log_message(f"[SYSTEM]: Файл {os.path.basename(filename)} загружен в буфер.")

    def copy_logs(self):
        self.clipboard_clear()
        self.clipboard_append(self.output_text.get("0.0", "end"))
        self.log_message("[GUI]: Логи скопированы в буфер обмена.")

    def run_process(self):
        target_path = self.input_entry.get()
        if not target_path or not os.path.exists(target_path):
            self.log_message("[ERROR]: Укажите корректный путь к файлу .safetensors!")
            return

        self.send_btn.configure(state="disabled")
        self.log_message("[RESONANCE]: Запуск процесса ввинчивания...")
        
        # Перенаправляем stdout для захвата логов из bridge_compressor
        class Redirector:
            def __init__(self, log_func): self.log_func = log_func
            def write(self, s): 
                if s.strip(): self.log_func(f"> {s.strip()}")
            def flush(self): pass

        old_stdout = sys.stdout
        sys.stdout = Redirector(self.log_message)

        try:
            # Запускаем логику из исправленного bridge_compressor
            bridge_compressor.main()
        except Exception as e:
            self.log_message(f"[CRITICAL ERROR]: {e}")
        finally:
            sys.stdout = old_stdout
            self.send_btn.configure(state="normal")
            self.log_message("[SYSTEM]: Цикл завершен.")

    def start_resonance(self):
        # Запуск в отдельном потоке, чтобы GUI не "замерзал"
        thread = threading.Thread(target=self.run_process)
        thread.daemon = True
        thread.start()

if __name__ == "__main__":
    app = MarkovShieldGUI()
    app.mainloop()
