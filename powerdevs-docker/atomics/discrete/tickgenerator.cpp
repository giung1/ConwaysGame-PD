#include "tickgenerator.h"
#include <stdlib.h> 

void tickgenerator::init(double t, ...) {
    va_list parameters;
    va_start(parameters, t);

    char* str_period = va_arg(parameters, char*);
    period = atof(str_period);

    va_end(parameters);

    if (period <= 0.0) period = 1.0;

    // Primer tick inmediato
    sigma = 0.0;
}

double tickgenerator::ta(double t) {
    return sigma;
}

void tickgenerator::dint(double t) {
    sigma = period;
}

void tickgenerator::dext(Event x, double t) {
    sigma = sigma - e;
    if (sigma < 0) sigma = 0;
}

Event tickgenerator::lambda(double t) {
    y[0] = 1.0;
    return Event(&y, 0);
}

void tickgenerator::exit() {
}