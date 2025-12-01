#include "cellgameoflife.h"
#include <stdlib.h> // Necesario para atoi (convertir texto a numero)

void cellgameoflife::init(double t, ...) {
    va_list parameters;
    va_start(parameters, t);

    // IMPORTANTE: El orden aqui debe coincidir EXACTAMENTE con tu PDM.
    // Asumiendo el orden: alive, isolating, overpopulation, toBorn, neighbors
    // Si tu PDM tiene un parametro "name" primero, debes agregar un va_arg extra al principio para ignorarlo.

    char* name = va_arg(parameters, char*); // Ignorado si ya tienes el nombre en 'name'
    // 1. Alive
    char* str_alive = va_arg(parameters, char*);
    double aliveInit = atof(str_alive); // Usamos atof por seguridad si viene como "1.0"
    alive = (aliveInit != 0.0) ? 1 : 0;
    next_alive = alive;

    // 2. Isolating
    char* str_iso = va_arg(parameters, char*);
    isolating = atoi(str_iso);

    // 3. Overpopulation
    char* str_over = va_arg(parameters, char*);
    overpopulation = atoi(str_over);

    // 4. ToBorn
    char* str_born = va_arg(parameters, char*);
    toBorn = atoi(str_born);

    // 5. Neighbors (Aunque no lo usaremos para el loop, hay que leerlo para limpiar la pila)
    char* str_neig = va_arg(parameters, char*);
    numNeighbors = atoi(str_neig);

    va_end(parameters);

    // Limpiamos la memoria de vecinos
    for (int i = 0; i < 8; ++i) {
        neighbors[i] = 0;
    }

    // Esto hace que la celula envie su estado inicial a los vecinos antes del primer Tick.
    sigma = 0.0;

    printLog("Cell %s initialized: alive=%d, isolating=%d, overpopulation=%d, toBorn=%d, numNeighbors=%d\n", 
             name, alive, isolating, overpopulation, toBorn, numNeighbors);
}

double cellgameoflife::ta(double t) {
    return sigma;
}

void cellgameoflife::dint(double t) {
    if (alive != next_alive){
        printLog("Cell %s internal transition: alive=%d -> next_alive=%d\n", name, alive, next_alive);
    }
    alive = next_alive;
    // Dormir hasta el proximo evento
    sigma = 1.0e20;
}

void cellgameoflife::dext(Event x, double t) {
    double *u = (double*)x.value;

    sigma = sigma - e;
    if (sigma < 0) sigma = 0;

    if (x.port == 0) { // Clock
        int k = 0;
        // Revisar SIEMPRE los 8 puertos posibles
        for (int i = 0; i < 8; ++i) {
            if (neighbors[i] != 0) ++k;
        }

        if (alive == 1) {
            if (k < isolating || k > overpopulation) next_alive = 0;
            else next_alive = 1;
        } else {
            if (k == toBorn) next_alive = 1;
            else next_alive = 0;
        }

        // Avisar cambio inmediatamente
        sigma = 0.0;

    } else if (x.port >= 1 && x.port <= 8) {
        // Actualizar vecino
        int idx = x.port - 1;
        if (idx >= 0 && idx < 8) {
            neighbors[idx] = (u[0] != 0.0) ? 1 : 0;
        }
    }
}

Event cellgameoflife::lambda(double t) {
    y[0] = (double)next_alive;
    return Event(&y, 0);
}

void cellgameoflife::exit() {
}