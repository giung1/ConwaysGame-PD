//CPP:discrete/tickgenerator.cpp
#if !defined tickgenerator_h
#define tickgenerator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

class tickgenerator : public Simulator {
// parámetros y estado
double sigma;
double period;
double y[10];

public:
    tickgenerator(const char *n) : Simulator(n) {};
    void init(double, ...);
    double ta(double t);
    void dint(double);
    void dext(Event, double);
    Event lambda(double);
    void exit();
};

#endif
