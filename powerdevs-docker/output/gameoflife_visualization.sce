// --- VISUALIZADOR DINÁMICO (Se ajusta solo) ---
archivo = 'salida_juego.txt';

// 1. Lectura de datos
try
    // Intentamos leer CSV ignorando la primera línea de texto
    raw = csvRead(archivo, ",", ".", "double", [], [], [], 1);
catch
    try
        raw = fscanfMat(archivo);
    catch
        messagebox("No puedo leer el archivo.", "Error");
        return;
    end
end

// 2. Detección Automática de Tamaño
[filas_archivo, columnas_archivo] = size(raw);

// La primera columna es tiempo, el resto son células
total_celulas = columnas_archivo - 1;

// Calculamos el lado del cuadrado (Raíz cuadrada del total)
// Si tu grilla NO es cuadrada (ej 5x10), cambia esto manualmente.
lado = sqrt(total_celulas); 

// Si la raíz no es exacta, avisamos (por seguridad)
if floor(lado) <> lado then
    disp("Advertencia: El número de células (" + string(total_celulas) + ") no forma un cuadrado perfecto.");
    // Si tu grilla no es cuadrada, descomenta y ajusta estas líneas:
    // lado_filas = 5;
    // lado_cols = 10;
else
    lado_filas = lado;
    lado_cols = lado;
end

disp("Detectado: " + string(lado_filas) + "x" + string(lado_cols) + " (" + string(total_celulas) + " celdas)");

// 3. Preparar datos
tiempo_registrado = raw(:, 1);
datos_registrados = raw(:, 2:$); // El $ significa "hasta el final" (lee todas las celdas)

t_final = max(tiempo_registrado);
paso_juego = 1; 
tiempos_a_dibujar = 0:paso_juego:t_final;

// --- 4. Configuración Gráfica (MODIFICADA) ---
dt_animacion = 0.2; 
f = scf(0); clf();

// DEFINICIÓN DE COLORES (R G B, valores de 0 a 1)
// Fila 1: Color para Células MUERTAS (0) -> Azul grisáceo (para que se distingan del fondo)
// Fila 2: Color para Células VIVAS (1)   -> Cian Brillante (o Amarillo si prefieres)
// Fila 3: Color del FONDO de la ventana  -> Azul Noche profundo
// Fila 4: Color de la REJILLA (Grid)     -> Gris claro
f.color_map = [0.20 0.25 0.40 ; ...  // Index 1: Muertas
               0.00 1.00 1.00 ; ...  // Index 2: Vivas
               0.05 0.05 0.20 ; ...  // Index 3: Fondo Ventana
               0.20 0.25 0.40 ];     // Index 4: Líneas Rejilla

// Asignamos el color 3 (Azul Noche) al fondo de la ventana gráfica
f.background = 3; 

// --- 5. Bucle ---
for t = tiempos_a_dibujar
    
    // (Lógica de búsqueda de datos - Igual que antes)
    indices = find(tiempo_registrado <= t + 0.0001);
    if isempty(indices) then
        vector_actual = zeros(1, total_celulas);
    else
        ultimo_idx = indices($);
        vector_actual = datos_registrados(ultimo_idx, :);
    end
    
    // Matriz
    matriz = matrix(vector_actual, lado_cols, lado_filas)';
    
    // DIBUJAR
    // Sumamos 1 para que: 0->Index 1 (Muerta), 1->Index 2 (Viva)
    Matplot(matriz + 1);
    
    // AJUSTES VISUALES (Aquí activamos la grilla)
    a = gca(); 
    a.isoview = "on"; 
    
    // Límites exactos
    a.data_bounds = [0.5, 0.5; lado_cols + 0.5, lado_filas + 0.5]; 
    
    // 1. Hacemos visibles los ejes (necesario para ver la grilla)
    a.axes_visible = ["off","off","off"]; 
    
    // 2. Activamos la rejilla usando el color 4 (Gris claro)
    // [color_x, color_y] -> Usamos el índice 4 de nuestro mapa de colores
    a.grid = [4 4]; 
    
    // Opcional: Si quieres que las marcas de los números no molesten, las ponemos en el color del fondo (truco visual)
    // O simplemente dejamos que se vean para saber las coordenadas.
    
    // Título en blanco (o cian) para que se lea sobre el azul oscuro
    title("Generación T = " + string(t), "color", "white", "fontsize", 3);
    
    sleep(dt_animacion * 200);
end
