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
  //testNeighbors();
  testGen();

  freecells();
  return 0;
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

void setcellAliveT(int x, int y){
  (temp[coord((x),(y))] = 1);
}

void setcellKillT(int x, int y){
  (temp[coord((x),(y))] = 0);
}

void copyCells(){
  memcpy(temp, cells, SIZE);
}

void copyTemp(){
  memcpy(cells,temp,SIZE);
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
/*
WINDOW *createwin(int height, int width, int begy, int begx)
{
   
    WINDOW *local_win = NULL;

  
    local_win = newwin(height, width, begy, begx);
    if(local_win == NULL) 
        exit(EXIT_FAILURE);

    wmove(local_win, 1, 1); 
    wrefresh(local_win); 

    return local_win;
}

void initcurses(){
  initscr();
  noecho();
  raw();

  keypad(stdscr, true);

  refresh();

  rows = LINES;
  cols = COLS - ICOLS;

  world = createwin(rows, cols, 0, 0);
  info = createwin(IROWS, ICOLS, 0, cols+1);

  box(info, 0, 0);
  mvwprintw(info, 1,1, "INFO");

  wmove(world, 1,1);

  wrefresh(info);
  wrefresh(world);
}

void status()
{
    int y,x;
    mvwprintw(info, 2, 1, "Total sizes");
    mvwprintw(info, 3, 1, " Columns: %d", COLS);
    mvwprintw(info, 4, 1, " Lines: %d", LINES);
    mvwprintw(info, 5, 1, "Life sizes");
    mvwprintw(info, 6, 1, " Columns: %d", cols);
    mvwprintw(info, 7, 1, " Lines: %d", rows);
    mvwprintw(info, 8, 1, "Array size: %d", CELLCOUNT);
    mvwprintw(info, 9, 1, "Tick size: %d", ticksize);
    getyx(world, y, x);
    wmove(world, y, x);
    wrefresh(info);
    wrefresh(world);
}
*/

void init(){

  //initcurses();

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

  /*
  delwin(info);
  delwin(world);

  endwin();
  */
}

int neighbours(int x, int y){
  int count = 0;
  if(y < (ARRAY_SIZE -1)){
    count += checkAlive(x,y+1);
  }
  if(y > 0){
    count += checkAlive(x,y-1);
  }
  if(x < (ARRAY_SIZE -1)){
    count += checkAlive(x+1, y);
    if(y > 0){
      count += checkAlive(x+1, y-1);      
    }
    if(y < (ARRAY_SIZE -1) ){
      count += checkAlive(x+1, y+1);
    }
  }
  if(x > 0){
    count += checkAlive(x-1, y);
    if(y > 0){
      count += checkAlive(x-1, y-1);
    }
    if(y < (ARRAY_SIZE -1) ){
      count += checkAlive(x-1, y+1);
    }
  }
  return count;
}

void nextGeneration(){
  int x,y,neigh;

  copyCells();
  for(int x = 0; x < ARRAY_SIZE; x++){
    for(int y = 0; y < ARRAY_SIZE; y++){
      neigh = neighbours(x,y);

      if(neigh < 2 || neigh > 3){
        //printf("%i,%i,kill\n",x,y);
        setcellKillT(x,y);
      }
      if(neigh == 3){
        //printf("%i,%i,alive\n",x,y);
        setcellAliveT(x,y);
      }
    }
  }
  copyTemp();
}

void testGen(){
  setcellAlive(2,4);
  setcellAlive(3,2);
  setcellAlive(3,4);
  setcellAlive(4,4);
  setcellAlive(4,3);
  
  printf("gen 0\n");
  displaycells();
  nextGeneration();
  printf("gen 1\n");
  displaycells();
  nextGeneration();
  printf("gen 2\n");
  displaycells();
  nextGeneration();
  printf("gen 3\n");
  displaycells();
}

void testNeighbors(){
  setcellAlive(3,3);
  setcellAlive(3,2);
  setcellAlive(3,4);

  printf("1,2, count %i\n",neighbours(1,2));
  printf("1,3, count %i\n",neighbours(1,2));
  printf("1,4, count %i\n",neighbours(1,2));
  printf("2,2, count %i\n",neighbours(1,2));
  printf("2,3, count %i\n",neighbours(1,2));
  printf("2,4, count %i\n",neighbours(1,2));
  displaycells();
}