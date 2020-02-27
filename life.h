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

#include "const.h"

//Pointer
int coord(int x, int y);
void setcellAlive(int x,int y);
void setcellAliveT(int x, int y);
void setcellKillT(int x, int y);

void copyCells(void);
void copyTemp();

void setcellKill(int x, int y);
int getcell(int x, int y);
int getPopulation();
void init();
void initcurses();
void randcell();
void displaycells();
int neighbours(int x, int y);
int checkAlive(int x, int y);
void freecells();
void status();
int convertx(int);
int converty(int);
void load_file(char*, int,int);


void nextGeneration();
void testGen();

void testNeighbors();


#endif //life_h
