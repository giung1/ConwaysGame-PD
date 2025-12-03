#include "cellgameoflife.h"
#include <stdlib.h> // Necesario para atoi (convertir texto a numero)

void cellgameoflife::init(double t, ...) {
    va_list parameters;
    va_start(parameters, t);


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

    va_end(parameters);

    // Limpiamos la memoria de vecinos
    for (int i = 0; i < 8; ++i) {
        neighbors[i] = 0;
    }

    // enviar estado inicial a los vecinos antes del primer Tick.
    sigma = 0.0;

    printLog("Cell %s initialized: alive=%d, isolating=%d, overpopulation=%d, toBorn=%d\n", 
             name, alive, isolating, overpopulation, toBorn);
}

double cellgameoflife::ta(double t) {
    return sigma;
}

void cellgameoflife::dint(double t) {
    if (alive != next_alive){
        printLog("Cell %s internal transition: alive=%d -> next_alive=%d\n", name, alive, next_alive);
    }
    alive = next_alive;
    // dormir hasta el proximo evento
    sigma = 1.0e20;
}

void cellgameoflife::dext(Event x, double t) {
    double *u = (double*)x.value;

    sigma = sigma - e;
    if (sigma < 0) sigma = 0;

    if (x.port == 0) { // Clock
        int k = 0;
        // revisar  los 8 puertos posibles
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

        // Aaisar cambio inmediatamente
        sigma = 0.0;

    } else if (x.port >= 1 && x.port <= 8) {
        // actualizar vecino
        int idx = x.port - 1;
        if (idx >= 0 && idx < 8) {
            neighbors[idx] = (u[0] != 0.0) ? 1 : 0;
        }
    }
}

Event cellgameoflife::lambda(double t) {
    if (t == 0.0) {
        y[0] = (double)alive; // esto lo hago para guardar el estado inicial en el archivo de salida, no influye en la simulación
    } else {
        y[0] = (double)next_alive;
    }
    return Event(&y, 0);
}

void cellgameoflife::exit() {
}