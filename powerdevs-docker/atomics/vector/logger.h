#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include <stdio.h>
#include <stdlib.h> 

class logger : public Simulator {
    double sigma;
    
    // CAMBIO 1: Puntero dinámico en lugar de array fijo
    double *valores; 
    
    // CAMBIO 2: Variable para guardar la cantidad total
    int num_celdas;  
    long pos_linea_actual; // Para guardar la posición del cursor en el archivo
    double tiempo_anterior; // Para saber si el tiempo cambió
    
    FILE *archivo;

public:
    logger(const char *n) : Simulator(n) {};
    void init(double, ...);
    double ta(double t);
    void dint(double);
    void dext(Event, double);
    Event lambda(double);
    void exit();
};