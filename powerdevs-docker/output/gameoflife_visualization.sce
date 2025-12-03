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
lado = sqrt(total_celulas); 
lado_filas = lado;
lado_cols = lado;

disp("Detectado: " + string(lado_filas) + "x" + string(lado_cols) + " (" + string(total_celulas) + " celdas)");

// 3. Preparar datos
tiempo_registrado = raw(:, 1);
datos_registrados = raw(:, 2:$); // El $ significa "hasta el final" (lee todas las celdas)

t_final = max(tiempo_registrado);
paso_juego = 1; 
tiempos_a_dibujar = 0:paso_juego:t_final;

dt_animacion = 0.2; 
f = scf(0); clf();


f.color_map = [0.20 0.25 0.40 ; ...  // Index 1: Muertas
               0.00 1.00 1.00 ; ...  // Index 2: Vivas
               0.05 0.05 0.20 ; ...  // Index 3: Fondo Ventana
               0.20 0.25 0.40 ];     // Index 4: Líneas Rejilla

f.background = 3; 

for t = tiempos_a_dibujar
    if find(winsid() == 0) == [] then
        break;
    end
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
    // Sumamos 1 para que 0->Index 1 (Muerta), 1->Index 2 (Viva)
    Matplot(matriz + 1);
    
    a = gca(); 
    a.isoview = "on"; 
    
    a.data_bounds = [0.5, 0.5; lado_cols + 0.5, lado_filas + 0.5]; 
    
    a.axes_visible = ["off","off","off"]; 
    

    a.grid = [4 4]; 
    

    
    title("Generación T = " + string(t), "color", "white", "fontsize", 3);
    
    sleep(dt_animacion * 500);
end
