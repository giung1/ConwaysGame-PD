//CPP:discrete/cellgameoflife.cpp
#if !defined cellgameoflife_h
#define cellgameoflife_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

class cellgameoflife : public Simulator {
// Estado, parámetros y salida
double sigma;          // tiempo hasta el próximo evento interno

int isolating;         // regla: soledad (normalmente 2)
int overpopulation;    // regla: sobrepoblación (normalmente 3)
int toBorn;            // regla: nacimiento (normalmente 3)

int alive;             // estado actual (0 o 1)
int next_alive;        // estado siguiente (0 o 1)
int neighbors[8];      // estado de cada vecino (0 o 1)

double y[10];          // vector de salida (igual que en qoperator_sci)

public:
    cellgameoflife(const char *n) : Simulator(n) {};
    void init(double, ...);
    double ta(double t);
    void dint(double);
    void dext(Event, double);
    Event lambda(double);
    void exit();
};

#endif
