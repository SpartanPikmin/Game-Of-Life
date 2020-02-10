#include "life.h"

int main(){
  initworld(world);
  initworld(tempworld);
  //displayworld();
  randworld();
  displayworld();
  //printf("hello world\n");
  return 0;
}

void initworld(int input[][ARRAY_SIZE]){
  for(int i = 0; i<ARRAY_SIZE; i++){
      for(int j = 0; j<ARRAY_SIZE; j++){
        input[i][j]=0;
      }
    }
}

void randworld(){
  for(int i = 0; i<=200; i++){
    int x = rand() % ARRAY_SIZE;
    int y = rand() % ARRAY_SIZE;
    world[x][y] = 1;
  }
}

void displayworld(){
    int i,j;

    for (i = 0; i < ARRAY_SIZE; i++) {
        //printf("%i: ", i);
        for (j = 0; j < ARRAY_SIZE; j++) {
            printf("%i", world[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}