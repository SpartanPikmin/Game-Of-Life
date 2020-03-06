#ifndef lifemp_h
#define lifemp_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#include "const.h"

#ifndef CELL
# define CELL uint8_t
# define UNITS long
#endif // CELL

UNITS coord(UNITS x, UNITS y);
void setcellAlive(UNITS x,UNITS y);
void setcellAliveT(UNITS x, UNITS y);
void setcellKillT(UNITS x, UNITS y);
void load_file(char*, UNITS,UNITS);
void mpNextGen(void);
void copyCells(void);
void copyTemp(void);
long convertx(long);
long converty(long);
void displaycells(void);
void setcellKill(UNITS x, UNITS y);
int getcell(UNITS x, UNITS y);
int checkAlive(UNITS x, UNITS y);
void init(void);
void freecells(void);
void mpNextGenFor(void);
int neighbours(UNITS x, UNITS y);


#endif