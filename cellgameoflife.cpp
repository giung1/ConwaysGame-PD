#include "cellgameoflife.h"

int cellID = 1;
int cellChecked = 0;

void cellgameoflife::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);
	DBL_MAX = 1.79769e+308;
	id = cellID++;

	printLog("PARAMETERS INITIALIZATION \n");

	char *fvar= va_arg(parameters,char*);
	name = fvar;

	fvar= va_arg(parameters,char*);
	isolating =  getScilabVar(fvar );
	
	fvar = va_arg(parameters,char*);
	overpopulation = getScilabVar(fvar );
	
	fvar = va_arg(parameters,char*);
	toBorn = getScilabVar(fvar );


	// printLog("isolating: %e\n", isolating);
	// printLog("overpopulation: %e\n", overpopulation);
	// printLog("toBorn: %e\n", toBorn);
	printLog("CELL ID: %d\n", id);
	// printLog("NAME: %s\n", name);

count = 0;
alive = 1;//rand() % 2;  
nextAlive = 0; 
dirty = 0; 
sumN = 0; 
sigma = 0.1; 

	// printLog("alive?: %d\n", alive);

}
double cellgameoflife::ta(double t) {
	printLog("TA %f \n", t);
return sigma;
}
void cellgameoflife::dint(double t) {
	printLog("DINT %d\n", id);
	if (t == 0.1){
		sigma = DBL_MAX;
		return;  
	}
	if( dirty == 1) {
		alive = nextAlive; 
		sumN = 0; 
		dirty = 0;
		sigma = DBL_MAX; // quedar pasiva hasta próximas entradas 
	}
}
void cellgameoflife::dext(Event x, double t) {
	int *v = (int*)x.value; // valor del vecino 
	sumN += *v;
	count++;
	printLog("DEXT No. %d on %d\n", count, id);
	// printLog("TIME: %e \n", t);
	// printLog("RECEIVED INPUT IN CELL name: %s \n", name);
	// printLog("VALUE RECEIVED: %d \n", *v);
	// printLog("SUM N NOW: %d \n", sumN);
	// printLog("COUNT NOW: %d \n", count);
	// printLog("_______________________________________________________________\n");

	if (alive == 1){
		nextAlive = (sumN < isolating || sumN > overpopulation) ? 0 : 1; 
	}
	else {
		nextAlive = (sumN == toBorn) ? 1 : 0; // si cambia, marcar dirty y emitir en lambda; después aplicar dirty = (nextAlive != alive) ? 1 : 0; // aplicar y limpiar 
	} 
		
	if (nextAlive != alive) {
		dirty = 1;
		if (count == 8 || 
		((id == 1 || id == 5 || id == 21 || id == 25) && count == 3) || //esquinas 
		(((id < 5 && id != 1) || (id > 21 && id != 25)) && count == 5)) { // bordes
			sigma = 0.1; // activar para próxima transición interna
			count = 0;
			dirty = 1;
		}
}

}
Event cellgameoflife::lambda(double t) {
	printLog("LAMBDA \n");
	// printLog("EMITTING FROM CELL name: %s \n", name);
	// printLog("STATUS FOR CELL: %d \n", alive);
	// printLog("STATUS FOR CELL next: %d \n", nextAlive);
	static int value;
	value = nextAlive;
	return Event(&value, 0);
}
void cellgameoflife::exit() {

}


// descubrir por que el valor que llega en dext es siempre 0.0
