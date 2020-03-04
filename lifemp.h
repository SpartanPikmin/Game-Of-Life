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

int coord(int x, int y);
void setcellAlive(int x,int y);
void setcellAliveT(int x, int y);
void setcellKillT(int x, int y);
void load_file(char*, int,int);
void mpNextGen();

void copyCells(void);
void copyTemp();
void displaycells();
void setcellKill(int x, int y);
int getcell(int x, int y);
int checkAlive(int x, int y);
void init();
void freecells();
int neighbours(int x, int y);


#endif