import customtkinter as ctk
from tkinter import filedialog

class MarkovShieldGUI(ctk.CTk):
    def __init__(self):
        super().__init__()

        # Цвета как на скрине
        self.BORDER_COLOR = "#7898d0"  # Серо-голубая рама
        self.INNER_BLUE = "#3c6cb4"   # Синий фон окон
        self.BTN_COLOR = "#5c8cd4"    # Цвет кнопок

        # Настройка главного окна (Широкое)
        self.title("MARKOV SHIELD V-CORE v149")
        self.geometry("1000x700")
        self.configure(fg_color=self.BORDER_COLOR)

        # ВЕРХНЕЕ ОКНО (Ответы ИИ)
        self.output_frame = ctk.CTkFrame(self, fg_color=self.INNER_BLUE, corner_radius=15, border_width=2, border_color="#FFFFFF")
        self.output_frame.pack(pady=(25, 10), padx=25, fill="both", expand=True)

        self.output_text = ctk.CTkTextbox(self.output_frame, fg_color="transparent", text_color="white", font=("Verdana", 14))
        self.output_text.pack(padx=10, pady=10, fill="both", expand=True)
        self.output_text.insert("0.0", "--- V-CORE v149: СИСТЕМА ГОТОВА ---\n")

        # НИЖНЕЕ ОКНО (Ввод и Кнопки - 1/4)
        self.input_frame = ctk.CTkFrame(self, height=160, fg_color=self.INNER_BLUE, corner_radius=15, border_width=2, border_color="#FFFFFF")
        self.input_frame.pack(pady=(0, 25), padx=25, fill="x")
        self.input_frame.pack_propagate(False)

        # Поле ввода
        self.input_entry = ctk.CTkEntry(self.input_frame, placeholder_text="Введите сообщение или путь...", 
                                        fg_color="transparent", border_width=0, text_color="white", font=("Verdana", 14))
        self.input_entry.pack(side="left", padx=20, fill="both", expand=True)

        # КОНТЕЙНЕР ДЛЯ КНОПОК (Справа внизу)
        self.btn_container = ctk.CTkFrame(self.input_frame, fg_color="transparent")
        self.btn_container.pack(side="right", padx=15, pady=15, anchor="s")

        # Параметры ПРЯМОУГОЛЬНЫХ кнопок со скруглением
        btn_params = {
            "width": 120, 
            "height": 38, 
            "fg_color": self.BTN_COLOR, 
            "corner_radius": 8, # Легкое скругление углов
            "border_width": 2, 
            "border_color": "#FFFFFF", 
            "text_color": "white",
            "font": ("Verdana", 10, "bold")
        }
        
        self.copy_btn = ctk.CTkButton(self.btn_container, text="COPY", **btn_params)
        self.copy_btn.pack(side="left", padx=5)

        self.load_btn = ctk.CTkButton(self.btn_container, text="LOAD", command=self.load_file, **btn_params)
        self.load_btn.pack(side="left", padx=5)

        self.send_btn = ctk.CTkButton(self.btn_container, text="SEND", **btn_params)
        self.send_btn.pack(side="left", padx=5)

    def load_file(self):
        filename = filedialog.askopenfilename()
        if filename:
            self.output_text.insert("end", f"\n[LOAD]: Файл {filename.split('/')[-1]} отправлен в Матрицу.\n")

if __name__ == "__main__":
    app = MarkovShieldGUI()
    app.mainloop()
