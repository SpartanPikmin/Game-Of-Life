#include "life.h"

int main(){
  /*
  initworld(world);
  initworld(tempworld);
  //displayworld();
  randworld();
  displayworld();
  //printf("hello world\n");
  */

  init();
  //displaycells();
  testNeighbors();

  freecells();
  return 0;
}

//ARRAYS
void initworld(int input[][ARRAY_SIZE]){
  for(int i = 0; i<ARRAY_SIZE; i++){
      for(int j = 0; j<ARRAY_SIZE; j++){
        input[i][j]=0;
      }
    }
}

void randworld(){
  for(int i = 0; i<=20; i++){
    int x = rand() % ARRAY_SIZE;
    int y = rand() % ARRAY_SIZE;
    world[x][y] = 1;
  }
}

void displayworld(){
    for (int i = 0; i < ARRAY_SIZE; i++) {
        //printf("%i: ", i);
        for (int j = 0; j < ARRAY_SIZE; j++) {
            printf("%i", world[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

//POINTERS 

int coord(int x, int y){
  return ((y)*ARRAY_SIZE+(x));
}
void setcellAlive(int x,int y){
  (cells[coord((x),(y))] = 1);
}
void setcellKill(int x, int y){
  (cells[coord((x),(y))] = 0);
}

int getcell(int x, int y){
  return (cells[coord(x,y)]);
}

int checkAlive(int x, int y){
  if(getcell(x,y) == 1){
    return true;
  }
  else{
    return false;
  }
}

int getPopulation(){
  int population = 0;
  for (int i = 0; i <= ARRAY_SIZE; i++){
    for(int j = 0; j <= ARRAY_SIZE; j++){
      if(getcell(i,j)){
        population++;
      }
    }
  }
  return population;
}

void randcell(){
  for(int i = 0; i<=20; i++){
    int x = rand() % ARRAY_SIZE;
    int y = rand() % ARRAY_SIZE;
    setcellAlive(x,y);
  }
}

void displaycells(){
  for (int i = 0; i < ARRAY_SIZE; i++) {
          //printf("%i: ", i);
          for (int j = 0; j < ARRAY_SIZE; j++) {
              printf("%i", getcell(i,j));
          }
          printf("\n");
      }
      printf("\n");
}

void init(){
  cells = malloc(SIZE);
  if(cells == NULL){
    exit(1);
  }
  memset(cells, 0, SIZE);

  temp = malloc(SIZE);
    if(temp == NULL)
        exit(1);
    memset(temp, 0, SIZE);
}

void freecells(){
  free(cells);
  free(temp);
}

int neighbours(int x, int y){
  int count = 0;
  if(y < (ARRAY_SIZE -1)){
    count += checkAlive(y+1,x);
  }
  if(y > 0){
    count += checkAlive(y-1,x);
  }
  if(x < (ARRAY_SIZE -1)){
    count += checkAlive(y, x+1);
    if(y > 0){
      count += checkAlive(y-1, x+1);
    }
    if(y < (ARRAY_SIZE -1) ){
      count += checkAlive(y+1, x+1);
    }
  }
  if(x > 0){
    count += checkAlive(y, x-1);
    if(y > 0){
      count += checkAlive(y-1, x-1);
    }
    if(y < (ARRAY_SIZE -1) ){
      count += checkAlive(y+1, x-1);
    }
  }
  return count;
}

void testNeighbors(){
  setcellAlive(3,3);
  setcellAlive(2,2);
  setcellAlive(2,3);
  setcellAlive(2,4);
  setcellAlive(3,2);
  setcellAlive(3,4);
  setcellAlive(4,2);
  setcellAlive(4,3);
  setcellAlive(4,4);

  if(neighbours(3,3) == 8){
    printf("neighbors is working\n");
  }
  else{
    printf("something is wrong\n");
  }
  displaycells();
}