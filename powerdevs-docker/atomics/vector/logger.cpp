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
        

        pos_linea_actual = ftell(archivo); 
    }
    
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
        valores[puerto] = (v[0] != 0.0) ? 1.0 : 0.0;
        
        if (archivo != NULL) {
            
            if (t == tiempo_anterior) {

                fseek(archivo, pos_linea_actual, SEEK_SET);
            } else {

                pos_linea_actual = ftell(archivo);
                tiempo_anterior = t;
            }


            fprintf(archivo, "%f", t);
            
            for(int i=0; i < num_celdas; i++) {
                fprintf(archivo, ", %d", (int)valores[i]);
            }
            fprintf(archivo, "\n");
            
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