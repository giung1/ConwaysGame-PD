import tkinter as tk
from tkinter import messagebox
import os 

# --- CONFIGURACIÓN DEL PDM ---
PATH = "../examples/gameoflife/"
FILENAME = "gameoflife_generated.pdm"
ATOMIC_PATH = "discrete/cellgameoflife.h"
TICK_PATH = "discrete/tickgenerator.h"
LOGGER_PATH = "vector/logger.h" 
LOGGER_BLOCK_NAME = "logger"

# Dimensiones visuales en PowerDEVS
CELL_W = 800
CELL_H = 800
SPACING_X = 1500
SPACING_Y = 1500 


def generar_pdm(rows, cols, alive_set, rules):
    """
    Genera el archivo PDM basado en la estructura que funciona.
    rules es un dict: {'iso': val, 'over': val, 'born': val}
    """
    lines = []
    
    # IDs de componentes
    total_cells = rows * cols
    tick_id = total_cells + 1
    logger_id = total_cells + 2

    # --- 1. CABECERA ---
    lines.append("Coupled")
    lines.append("    {")
    lines.append("    Type = Root")
    lines.append(f"    Name = GameOfLife_{rows}x{cols}")
    lines.append("    Ports = 0 ; 0")
    lines.append("    Description = Generado por Script Python")
    lines.append("    Graphic")
    lines.append("        {")
    lines.append("        Position = 0 ; 0")
    lines.append("        Dimension = 600 ; 600")
    lines.append("        Direction = Right")
    lines.append("        Color = 15")
    lines.append("        Icon = ")
    lines.append("        Window = 5000 ; 5000 ; 5000 ; 5000")
    lines.append("        }")
    lines.append("    Parameters")
    lines.append("        {")
    lines.append("        }")
    lines.append("    System")
    lines.append("        {")

    # --- 2. CELDAS ATÓMICAS ---
    for r in range(rows):
        for c in range(cols):
            cell_idx = (r * cols) + c
            
            # Coordenadas visuales (Grid)
            pos_x = c * SPACING_X
            pos_y = r * SPACING_Y 
            
            # Estado
            is_alive = 1 if (r, c) in alive_set else 0
            
            cell_name = f"C{r+1}{c+1}" 

            lines.append("        Atomic")
            lines.append("            {")
            lines.append(f"            Name = {cell_name}")
            lines.append("            Ports = 9 ; 1")
            lines.append(f"            Path = {ATOMIC_PATH}")
            lines.append(f"            Description = Celda ({r+1},{c+1})")
            lines.append("            Graphic")
            lines.append("                {")
            lines.append(f"                Position = {pos_x} ; {pos_y}")
            lines.append(f"                Dimension = {CELL_W} ; {CELL_H}")
            lines.append("                Direction = Right")
            lines.append("                Color = 15")
            lines.append("                Icon = ../assets/cell.jpg")
            lines.append("                }")
            lines.append("            Parameters")
            lines.append("                {")
            lines.append(f"                name = Str; {cell_name} ; ")
            lines.append(f"                alive = Str; {is_alive} ; ")
            lines.append(f"                isolating = Str; {rules['iso']} ; ")
            lines.append(f"                overpopulation = Str; {rules['over']} ; ")
            lines.append(f"                toBorn = Str; {rules['born']} ; ")
            lines.append("                }")
            lines.append("            }")

    # --- 3. TICK GENERATOR ---
    lines.append("        Atomic")
    lines.append("            {")
    lines.append("            Name = Tick")
    lines.append("            Ports = 0 ; 1")
    lines.append(f"            Path = {TICK_PATH}")
    lines.append("            Description = Generador de ticks")
    lines.append("            Graphic")
    lines.append("                {")
    lines.append(f"                Position = -2000 ; 0") 
    lines.append(f"                Dimension = {CELL_W} ; {CELL_H}")
    lines.append("                Direction = Right")
    lines.append("                Color = 15")
    lines.append("                Icon = ../assets/tick.png")
    lines.append("                }")
    lines.append("            Parameters")
    lines.append("                {")
    lines.append("                period = Str; 1 ; ")
    lines.append("                }")
    lines.append("            }")

    # --- 4. LOGGER ---
    lines.append("        Atomic")
    lines.append("            {")
    lines.append(f"            Name = {LOGGER_BLOCK_NAME}")
    lines.append(f"            Ports = {total_cells} ; 0") 
    lines.append(f"            Path = {LOGGER_PATH}")
    lines.append("            Description = Logger automatico")
    lines.append("            Graphic")
    lines.append("                {")
    lines.append(f"                Position = {cols * SPACING_X + 2000} ; 0") 
    lines.append(f"                Dimension = 3500 ; 3500")
    lines.append("                Direction = Right")
    lines.append("                Color = 15")
    lines.append("                Icon = ../assets/logger.png")
    lines.append("                }")
    lines.append("            Parameters")
    lines.append("                {")
    lines.append("                Index = Str; 0 ; ")
    lines.append(f"                Size = Str; {total_cells} ; ")
    lines.append("                }")
    lines.append("            }")

    # --- 5. CONEXIONES ---
    # A) Tick -> Celdas
    for i in range(total_cells):
        lines.append("        Line")
        lines.append("            {")
        lines.append(f"            Source = Cmp ; {tick_id} ; 1 ; 0") 
        lines.append(f"            Sink = Cmp ; {i+1} ; 1 ; -1")    
        lines.append("            PointX = -1500 ; 0") 
        lines.append("            PointY = 0 ; 0")
        lines.append("            }")

    # B) Celdas -> Logger
    for i in range(total_cells):
        lines.append("        Line")
        lines.append("            {")
        lines.append(f"            Source = Cmp ; {i+1} ; 1 ; 0") 
        lines.append(f"            Sink = Cmp ; {logger_id} ; {i+1} ; -1") 
        lines.append("            PointX = 0 ; 2000")
        lines.append("            PointY = 0 ; 0")
        lines.append("            }")

    # C) Vecinos
    port_mapping = {
        (-1, -1): 2, (-1, 0):  3, (-1, 1):  4,
        (0, -1):  5,              (0, 1):   6,
        (1, -1):  7, (1, 0):   8, (1, 1):   9
    }

    for r in range(rows):
        for c in range(cols):
            my_id = (r * cols) + c + 1
            for dr in [-1, 0, 1]:
                for dc in [-1, 0, 1]:
                    if dr == 0 and dc == 0: continue
                    nr, nc = r + dr, c + dc
                    if 0 <= nr < rows and 0 <= nc < cols:
                        neighbor_id = (nr * cols) + nc + 1
                        input_port = port_mapping[(dr, dc)]
                        lines.append("        Line")
                        lines.append("            {")
                        lines.append(f"            Source = Cmp ; {neighbor_id} ; 1 ; 0")
                        lines.append(f"            Sink = Cmp ; {my_id} ; {input_port} ; -1")
                        lines.append("            PointX = 0 ; 0")
                        lines.append("            PointY = 0 ; 0")
                        lines.append("            }")

    lines.append("        }")
    lines.append("    }")

    # --- ESCRITURA DEL ARCHIVO EN PATH ESPECIFICO ---
    
    # 2. Construir la ruta completa de manera segura
    full_path = os.path.join(PATH, FILENAME)

    try:
        os.makedirs(PATH, exist_ok=True)
    except OSError as e:
        print(f"Advertencia: No se pudo crear el directorio {PATH}. {e}")

    # Usar full_path en el open
    try:
        with open(full_path, "w") as f:
            f.write("\n".join(lines))
        print(f"Archivo generado correctamente en: {full_path}")
        # Retornamos True o el path para usarlo en el mensaje de éxito de la GUI
        return full_path
    except Exception as e:
        print(f"Error al escribir el archivo: {e}")
        return None

# --- GUI ---
class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Generador PDM - Game of Life")
        self.geometry("600x500")
        
        # --- TRUCO PARA PIXELES (Visto en la pregunta anterior) ---
        self.pixel = tk.PhotoImage(width=1, height=1)

        # --- 1. PANEL SUPERIOR ---
        top_frame = tk.Frame(self)
        top_frame.pack(side="top", fill="x", pady=5)

        # Configuración Filas/Cols
        f_config = tk.Frame(top_frame)
        f_config.pack(pady=5)
        
        tk.Label(f_config, text="Filas y Columnas:").grid(row=0, column=0)
        self.entry_rows_cols = tk.Entry(f_config, width=5)
        self.entry_rows_cols.insert(0, "10")
        self.entry_rows_cols.grid(row=0, column=1)
        
        
        tk.Button(f_config, text="Crear Grid", command=self.create_grid).grid(row=0, column=4, padx=10)
        
        # Reglas
        f_rules = tk.LabelFrame(top_frame, text="Reglas del Juego")
        f_rules.pack(pady=5, padx=10)
        
        tk.Label(f_rules, text="Isolating (<):").pack(side="left")
        self.e_iso = tk.Entry(f_rules, width=3)
        self.e_iso.insert(0, "2")
        self.e_iso.pack(side="left", padx=5)
        
        tk.Label(f_rules, text="Overpop (>):").pack(side="left")
        self.e_over = tk.Entry(f_rules, width=3)
        self.e_over.insert(0, "3")
        self.e_over.pack(side="left", padx=5)
        
        tk.Label(f_rules, text="To Born (==):").pack(side="left")
        self.e_born = tk.Entry(f_rules, width=3)
        self.e_born.insert(0, "3")
        self.e_born.pack(side="left", padx=5)

        # --- 2. PANEL CENTRAL (SCROLL) ---
        self.container_frame = tk.Frame(self, bd=2, relief="sunken")
        self.container_frame.pack(side="top", fill="both", expand=True, padx=10, pady=5)
        
        self.container_frame.grid_rowconfigure(0, weight=1)
        self.container_frame.grid_columnconfigure(0, weight=1)

        self.canvas = tk.Canvas(self.container_frame, bg="#f0f0f0")
        self.v_scroll = tk.Scrollbar(self.container_frame, orient="vertical", command=self.canvas.yview)
        self.h_scroll = tk.Scrollbar(self.container_frame, orient="horizontal", command=self.canvas.xview)
        
        self.canvas.configure(yscrollcommand=self.v_scroll.set, xscrollcommand=self.h_scroll.set)

        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.v_scroll.grid(row=0, column=1, sticky="ns")
        self.h_scroll.grid(row=1, column=0, sticky="ew")

        self.f_grid = tk.Frame(self.canvas, bg="#f0f0f0")
        
        # --- CAMBIO IMPORTANTE: Guardamos el ID de la ventana ---
        self.canvas_window = self.canvas.create_window((0, 0), window=self.f_grid, anchor="nw")

        # --- BINDINGS PARA CENTRADO ---
        # Detectar cuando cambia el tamaño del Canvas (ventana)
        self.canvas.bind("<Configure>", self._on_canvas_configure)
        # Detectar cuando cambia el tamaño del Grid (contenido)
        self.f_grid.bind("<Configure>", self._on_frame_configure)
        
        self.canvas.bind_all("<MouseWheel>", self._on_mousewheel)

        # --- 3. PANEL INFERIOR ---
        bottom_frame = tk.Frame(self)
        bottom_frame.pack(side="bottom", fill="x", pady=10)
        
        tk.Button(bottom_frame, text="GENERAR PDM", bg="green", fg="white", font=("Arial", 10, "bold"),
                  command=self.generate).pack(pady=5, padx=20, fill="x")
        
        self.cells = {} 
        self.create_grid()

    def _on_mousewheel(self, event):
        self.canvas.yview_scroll(int(-1*(event.delta/120)), "units")

    
    def _center_content(self):
        """Calcula la posición para mantener el grid centrado o en (0,0)"""
        self.canvas.update_idletasks() # Asegura que las medidas sean actuales
        
        # Medidas del Canvas (visible)
        cw = self.canvas.winfo_width()
        ch = self.canvas.winfo_height()
        
        # Medidas del Grid (contenido)
        fw = self.f_grid.winfo_reqwidth()
        fh = self.f_grid.winfo_reqheight()

        # Si el canvas es más grande que el grid, calculamos el offset para centrar
        # Si es más chico, el offset es 0 para permitir scroll normal
        x_offset = max(0, (cw - fw) // 2)
        y_offset = max(0, (ch - fh) // 2)

        # Movemos la ventana interna del canvas a esa posición
        self.canvas.coords(self.canvas_window, x_offset, y_offset)
        
        # Actualizamos la región de scroll para que coincida con el contenido + el offset
        self.canvas.configure(scrollregion=(0, 0, fw + x_offset*2, fh + y_offset*2))

    def _on_canvas_configure(self, event):
        """Se ejecuta cuando redimensionas la ventana principal"""
        self._center_content()

    def _on_frame_configure(self, event):
        """Se ejecuta cuando cambia el tamaño del grid interno"""
        self._center_content()

    def create_grid(self):
        # Limpiar frame interno
        for widget in self.f_grid.winfo_children():
            widget.destroy()
        self.cells = {}
        
        # --- ESTETICA DEL FONDO ---
        # Cambiamos el fondo del frame grid a oscuro para que se vea bien
        self.f_grid.config(bg="#222222") 
        
        try:
            r = int(self.entry_rows_cols.get())
            c = int(self.entry_rows_cols.get())
        except: return

        for i in range(r):
            for j in range(c):
                # Creamos el botón
                # bg="black" -> Fondo negro por defecto
                # activebackground="#006400" -> Color al presionar (verde oscuro)
                # bd=1, relief="solid" -> Borde fino para distinguir celdas negras entre si
                btn = tk.Button(self.f_grid, 
                                image=self.pixel, 
                                width=25, height=25, 
                                bg="black",
                                activebackground="#006400",
                                bd=1, relief="solid",
                                command=lambda x=i, y=j: self.toggle(x,y))
                
                btn.grid(row=i, column=j, padx=0, pady=0) # padx=0 para que estén pegadas (opcional)
                
                # --- EVENTOS HOVER ---
                # Pasamos i, j para saber qué celda es
                btn.bind("<Enter>", lambda event, row=i, col=j: self.on_enter(row, col))
                btn.bind("<Leave>", lambda event, row=i, col=j: self.on_leave(row, col))

                self.cells[(i,j)] = {"btn": btn, "alive": False}
        
        self.f_grid.update_idletasks()
        self._center_content()

    # --- NUEVAS FUNCIONES DE HOVER ---

    def on_enter(self, r, c):
        if self.cells[(r,c)]["alive"]:
            return # Si está viva no hacemos nada
        self.cells[(r,c)]["btn"].config(bg="#006400") # Dark Green (Hover en muerta)

    def on_leave(self, r, c):
        is_alive = self.cells[(r,c)]["alive"]
        if is_alive:
            self.cells[(r,c)]["btn"].config(bg="#00ff00") # Lime Green (Viva)
        else:
            self.cells[(r,c)]["btn"].config(bg="black")   # Negro (Muerta)

    def toggle(self, r, c):
        alive = not self.cells[(r,c)]["alive"]
        self.cells[(r,c)]["alive"] = alive
        
        # Feedback visual inmediato
        if alive:
            # Si revive -> Verde Brillante
            self.cells[(r,c)]["btn"].config(bg="#00ff00")
        else:
            # Si muere -> Verde Oscuro 
            # Ponemos verde oscuro porque el mouse sigue encima de la celda.
            # Cuando muevas el mouse fuera, el evento on_leave la pondrá negra.
            self.cells[(r,c)]["btn"].config(bg="#006400")


    def generate(self):
        try:
            rows = int(self.entry_rows_cols.get())
            cols = int(self.entry_rows_cols.get())
            rules = {
                "iso": self.e_iso.get(),
                "over": self.e_over.get(),
                "born": self.e_born.get()
            }
        except:
            messagebox.showerror("Error", "Revisa los números ingresados")
            return

        alive_set = set()
        for (r,c), data in self.cells.items():
            if data["alive"]:
                alive_set.add((r,c))
        
        # Llamamos a la función y verificamos el retorno
        saved_path = generar_pdm(rows, cols, alive_set, rules)
        
        if saved_path:
            messagebox.showinfo("Éxito", f"Archivo generado en:\n{saved_path}\n({len(alive_set)} células vivas).")
        else:
            messagebox.showerror("Error", "No se pudo guardar el archivo.")

if __name__ == "__main__":
    app = App()
    app.mainloop()