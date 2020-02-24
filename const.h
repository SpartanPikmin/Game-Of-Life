#ifndef const_h
#define const_h

#define true 1
#define false 0

#define sleep 2000000

const int SCREEN_WIDTH = 100;
const int SCREEN_HIGHT = 100;

const int ARRAY_SIZE = 10;

#define ICOLS 30
#define IROWS LINES

#define RMAX (rows-1);
#define CMAX (cols-1);

#define CELLCOUNT (ARRAY_SIZE*ARRAY_SIZE)
#define SIZE (CELLCOUNT*sizeof(int))

#endif