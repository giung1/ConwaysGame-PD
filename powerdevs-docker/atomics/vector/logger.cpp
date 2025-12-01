#include "logger.h"
#include <string.h>

void logger::init(double t, ...) {
    va_list parameters;
    va_start(parameters, t);

    char* dummy_index = va_arg(parameters, char*);
    char* str_size = va_arg(parameters, char*);
    num_celdas = atoi(str_size); 
    
    va_end(parameters);

    if (num_celdas <= 0) num_celdas = 9; 

    valores = new double[num_celdas];

    for(int i=0; i < num_celdas; i++) {
        valores[i] = 0;
    }

    archivo = fopen("salida_juego.txt", "w");
    
    if (archivo != NULL) {
        fprintf(archivo, "Tiempo");
        for(int i=0; i < num_celdas; i++) {
            fprintf(archivo, ", C%d", i+1);
        }
        fprintf(archivo, "\n");
        
        // --- CAMBIO 1: Inicialización de control de archivo ---
        // Guardamos dónde termina el encabezado para empezar a escribir datos ahí
        pos_linea_actual = ftell(archivo); 
    }
    
    // Inicializamos con un tiempo imposible para asegurar que el primer t=0 se escriba bien
    tiempo_anterior = -1.0; 

    sigma = 1e20;
}

double logger::ta(double t) {
    return sigma;
}

void logger::dint(double t) {
    sigma = 1e20;
}

void logger::dext(Event x, double t) {
    int puerto = x.port;
    double *v = (double*)x.value;
    
    if (puerto >= 0 && puerto < num_celdas) {
        // Actualizamos el estado interno
        valores[puerto] = (v[0] != 0.0) ? 1.0 : 0.0;
        
        if (archivo != NULL) {
            // --- CAMBIO 2: Lógica de Sobrescritura ---
            
            if (t == tiempo_anterior) {
                // Si es el MISMO tiempo, rebobinamos el archivo al inicio de esta línea
                // para sobrescribirla con los datos actualizados.
                fseek(archivo, pos_linea_actual, SEEK_SET);
            } else {
                // Si es un tiempo NUEVO, guardamos la posición actual antes de escribir.
                // Esta será la posición de retorno si llega otro evento en este mismo t.
                pos_linea_actual = ftell(archivo);
                tiempo_anterior = t;
            }

            // Escribimos la línea (ya sea nueva o sobrescribiendo la vieja)
            fprintf(archivo, "%f", t);
            
            for(int i=0; i < num_celdas; i++) {
                fprintf(archivo, ", %d", (int)valores[i]);
            }
            fprintf(archivo, "\n");
            
            // Forzamos la escritura al disco por si el programa se cierra abruptamente
            fflush(archivo); 
        }
    }
    sigma = sigma - e;
}

Event logger::lambda(double t) {
    return Event();
}

void logger::exit() {
    if (archivo != NULL) {
        fclose(archivo);
    }
    if (valores != NULL) {
        delete[] valores;
    }
}