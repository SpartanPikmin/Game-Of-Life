#ifndef life_h
#define life_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdlib.h>
#include "const.h"

int main();

//array
void randworld();
void initworld(int Array[][ARRAY_SIZE]);
void displayworld();

//Pointer
int coord(int x, int y);
void setcellAlive(int x,int y);
void setcellKill(int x, int y);
int getcell(int x, int y);
int getPopulation();
void init();
void randcell();
void displaycells();
int neighbours(int x, int y);
int checkAlive(int x, int y);
void freecells();

void nextGeneration();

void testNeighbors();


static int *cells = NULL;
static int *temp = NULL;
int world[ARRAY_SIZE][ARRAY_SIZE];
int tempworld[ARRAY_SIZE][ARRAY_SIZE];




#endif //life_h

