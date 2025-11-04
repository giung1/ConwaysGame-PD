//CPP:random/lib/randomc.cpp
//CPP:discrete/cellgameoflife.cpp
#if !defined cellgameoflife_h
#define cellgameoflife_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "random/lib/randomc.h"

extern int cellChecked;

class cellgameoflife: public Simulator { 
char* name;
int id;
double DBL_MAX;
int count;
int alive;
int nextAlive;
int dirty;
int sumN;
double sigma;
double isolating;
double overpopulation;
double toBorn;
public:
	cellgameoflife(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
