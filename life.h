#ifndef life_h
#define life_h
#include <stdio.h>
#include <stdlib.h>
#include "const.h"

int main();

void randworld();
void initworld(int Array[][ARRAY_SIZE]);
void displayworld();

int world[ARRAY_SIZE][ARRAY_SIZE];
int tempworld[ARRAY_SIZE][ARRAY_SIZE];



#endif //life_h

