#include "lifemp.h"

static int *cells = NULL;
static int *temp = NULL;

int ticks = 1;

int cols = 0;
int rows = 0;

int NUM_THREADS;

int CELLCOUNT;
int SIZE;
int Verbose = 0;
  
struct shapefile{
  char *fileName;
  int x;
  int y;
};


/*
  do next gen with serial
  do next gen with openmp for loop



*/

int main(int argc, char *argv[]){
  
  int opt;
  int file_loc = 0;
  int fx = 0, fy = 0;
  char *file = NULL;
  char stop;
  int j;
  struct shapefile filearray[10];
  while((opt = getopt(argc, argv, "c:r:t:n:x:y:f:v")) != -1 ){
      switch(opt){
        case'c':
          cols = (int) strtol(optarg, NULL,10);
          break;
        case'r':
          rows = (int) strtol(optarg, NULL,10);
          break;
        case't':
          ticks = (int) strtol(optarg, NULL,10);
          break;
        case'n':
          NUM_THREADS = (int) strtol(optarg, NULL,10);
          break;
        case 'f':
        //printf("got optarg\n");
          file = optarg;
          filearray[file_loc].fileName = file;
          file_loc++;
          //printf("entering method\n");
          break;
        case 'x':
          fx = atoi(optarg);
          filearray[file_loc].x = fx;
          break;
        case 'y':
          fy = atoi(optarg);
          filearray[file_loc].y = fy;
          break;
        case 'v':
          Verbose ++;
          break;
          }
  }
  if(Verbose > 0){
    printf("%i %i %i %i\n\n", cols, rows, ticks, NUM_THREADS);
  }

  CELLCOUNT = (cols * rows);
  SIZE = (CELLCOUNT * sizeof(int));
  if(Verbose > 0){
    printf("creating world\n");
    }   
  init();
  for(j = 0; j < file_loc; j++){
    printf("loading file at location %i\n", j);
    load_file(filearray[j].fileName,filearray[j].x,filearray[j].y);
  }
  //displaycells();

  //printf("seting numbers\n");
  omp_set_num_threads(NUM_THREADS);
  //printf("entering method\n");
  mpNextGen();
    
  return 0;
}

void mpNextGen(){
  //printf("in method\n");
  #pragma omp parallel
    {
      int ID = omp_get_thread_num();
      int i,x,y,neigh;
      for(i = 0; i < ticks; i++){
        #pragma omp single
        {
          //printf("copyC");
          copyCells();
        }
        //printf("%i waiting",ID);
        #pragma omp barrier
        for(x = ID; x < rows; x += NUM_THREADS){
          //printf("1\n");
          for(y = 0; y < cols; y++){
            neigh = neighbours(x,y);
            //printf("2\n");
            if(neigh < 2 || neigh > 3){
              //printf("3\n");
              setcellKillT(x,y);
              
            }
            if(neigh == 3){
             // printf("4\n");
              setcellAliveT(x,y);
              
            }
          }
        }
        //printf("5\n");
        #pragma omp barrier
        #pragma omp single
        {
          //printf("6\n");
          copyTemp();
          if(Verbose > 0){
            displaycells();
          }
        }
        //printf("7\n");
      }
      //printf("8\n");
  }
  //printf("9\n");
}


void init(){

  cells = malloc(SIZE);
  if(cells == NULL){
    exit(1);
  }
  memset(cells, 0, SIZE);

  temp = malloc(SIZE);
    if(temp == NULL){
      exit(1);
    }      
  memset(temp, 0, SIZE);
 // printf("in init\n");
  //displaycells();
}

int neighbours(int x, int y){
  int count = 0;
  if(y < (cols -1)){
    //printf("count 1\n");
    count += checkAlive(convertx(x),converty(y+1));
  }
  if(y > 0){
    //printf("count 2\n");
    count += checkAlive(convertx(x),converty(y-1));
  }
  if(x < (rows -1)){
   // printf("count 3\n");
    count += checkAlive(convertx(x+1), converty(y));
    if(y > 0){
      //printf("count 4\n");
      count += checkAlive(convertx(x+1), converty(y-1));      
    }
    if(y < (cols -1) ){
     // printf("count 5\n");
      count += checkAlive(convertx(x+1),converty(y+1));
    }
  }
  if(x > 0){
    //printf("count 6\n");
    count += checkAlive(convertx(x-1), converty(y));
    if(y > 0){
     // printf("count 7\n");
      count += checkAlive(convertx(x-1), converty(y-1));
    }
    if(y < (cols-1) ){
      //printf("count 8\n");
      count += checkAlive(convertx(x-1), converty(y+1));
    }
  }
  return count;
}

void freecells(){
  printf("free\n");
  free(cells);
  free(temp);

}

void displaycells(){
  int i,j;
  for (i = 0; i < rows; i++) {
          //printf("%i: ", i);
          for (j = 0; j < cols; j++) {
            if(Verbose > 1 ){
              if (getcell(i,j) != 0 && getcell(i,j) != 1) {
                  fprintf(stderr, "there is a corruption at %i %i\n", i, j);
              }
              }
              
              printf("%i", getcell(i,j));
          }
          printf("\n");
      }
      printf("\n");
}

int coord(int x, int y){
  return ((y)*cols+(x));
}
void setcellAlive(int x,int y){
  (cells[coord((x),(y))] = 1);
  //printf("cell x %i y %i is alive\n", x, y);
  //displaycells();
}
void setcellKill(int x, int y){
  (cells[coord((x),(y))] = 0);
  //printf("cell x %i y %i is dead\n", x, y);
  //sdisplaycells();
}

void setcellAliveT(int x, int y){
  (temp[coord((x),(y))] = 1);
}

void setcellKillT(int x, int y){
  (temp[coord((x),(y))] = 0);
}

void copyCells(void){
  memcpy(temp, cells, SIZE);
}

void copyTemp(){
  //printf("hello\n");
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

int convertx(int x){
  int new_x = x;

  if(x < 0){
    new_x = rows - 1;
  }
  else if(x >= rows){
    new_x = 0;
  }
  return new_x;
}

int converty(int y){
  int new_y = y;

  if(y < 0){
    new_y = cols - 1;
  }
  else if(y >= cols){
    new_y = 0;
  }
  return new_y;
}

void load_file(char* file, int x, int y){
  /*
  if(!created){
   printf("creating world\n");
    init();
    displaycells();
    created = true;
  }
  */
  //printf("in method\n");
  //printf("%s",file);
  char str[100] = {0};
  FILE *iFile = fopen(file, "r");

  //printf("get file\n");
  //iFile = fopen(file, "r");
  //printf("got file\n");

  if(iFile == NULL){
    printf("there was an error opening that file\n");
    sleep(2);
  }
  else{
   // printf("in else\n");
    int curx = x;
    int cury = y;

    while(fgets(str, sizeof(str),iFile) != NULL){
      //printf("loop\n");
     //printf("%s\n",str);
      int i;
      curx = x;

      for(i = 0; i < strlen(str); i++){
        switch(str[i]){
          case '\n':
          case '\t':
          case '\r':
            //ignore
            break;
          case ' ':
          case '.':
            //printf("thing\n");
            setcellKill(curx,cury);
            //printf("cell %i %i value %i dead\n", curx,cury, checkAlive(curx,curx));
            break;
          default:
           //printf("other thing\n");
            setcellAlive(curx,cury);
            //printf("cell %i %i value %i alive\n", curx,cury, getcell(curx,cury));
            break;
        }
        curx = convertx(curx + 1);
      }
      cury = converty(cury + 1);

    }
    //printf("about to close\n");

    fclose(iFile);

    //printf("closed\n");
  }

}