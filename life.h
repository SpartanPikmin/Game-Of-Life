#ifndef life_h
#define life_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdlib.h>
#include "const.h"
#include <locale.h>
#include <time.h>

int main();

//Pointer
int coord(int x, int y);
void setcellAlive(int x,int y);
void setcellAliveT(int x, int y);
void setcellKillT(int x, int y);

void copyCells();
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
//WINDOW *createwin(int height, int width, int begy, int begx);


void status();

void nextGeneration();
void testGen();

void testNeighbors();


static int *cells = NULL;
static int *temp = NULL;

static int ticksize = 1;

static int cols = 0;
static int rows = 0;



#endif //life_h

