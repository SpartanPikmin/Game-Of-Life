#ifndef life_h
#define life_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include "const.h"

long coord(long x, long y);
void setcellAlive(long x,long y);
void setcellAliveT(long x, long y);
void setcellKillT(long x, long y);
void copyCells(void);
void copyTemp(void);
void setcellKill(long x, long y);
int getcell(long x, long y);
int checkAlive(long x, long y);
int getPopulation(void);
void init(void);
void randcell(void);
void displaycells(void);
int neighbours(long x, long y);
void freecells(void);
void status(void);
long convertx(long);
long converty(long);
void load_file(char*, long,long);
void nextGeneration(void);
void testGen(void);
void testNeighbors(void);
#endif //life_h