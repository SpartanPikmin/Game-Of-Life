#include "lifemp.h"

#ifndef CELL
# define CELL uint8_t
# define UNITS long
#endif // CELL

static CELL *cells = NULL;
static CELL *temp = NULL;

int ticks = 1;

UNITS cols = 0;
UNITS rows = 0;

int NUM_THREADS;

UNITS CELLCOUNT;
UNITS SIZE;
int Verbose = 0;

clock_t start, world_gen,file_time, end;
  
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
  int j;
  struct shapefile filearray[10];
  start = clock();
  while((opt = getopt(argc, argv, "c:r:t:n:x:y:f:v")) != -1 ){
      switch(opt){
        case'c':
          cols = strtol(optarg, NULL,10);
          break;
        case'r':
          rows = strtol(optarg, NULL,10);
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
    printf("%li %li %i %i\n\n", cols, rows, ticks, NUM_THREADS);
  }

  CELLCOUNT = (cols * rows);
  SIZE = (CELLCOUNT * sizeof(CELL));
  if(Verbose > 0){
    printf("creating world\n");
    }   
  init();
  world_gen = clock();
  for(j = 0; j < file_loc; j++){
    if(Verbose > 1){
      printf("loading file at location %i\n", j);
    } 
    load_file(filearray[j].fileName,filearray[j].x,filearray[j].y);
  }
  file_time = clock();
  //displaycells();

  //printf("seting numbers\n");
  omp_set_num_threads(NUM_THREADS);
  //printf("entering method\n");
  mpNextGen();
  end = clock();
  printf("start\tworld gen\tfile place\tthread time\n");
  printf("%f\t%f\t%f\t%f\n",(((double)(end-start))/CLOCKS_PER_SEC), (((double)(world_gen-start))/CLOCKS_PER_SEC)
                            , (((double)(file_time-world_gen))/CLOCKS_PER_SEC), (((double)(end-file_time))/CLOCKS_PER_SEC));
  return 0;
}
UNITS convertx(UNITS x){
  UNITS new_x = x;

  if(x < 0){
    new_x = rows - 1;
  }
  else if(x >= rows){
    new_x = 0;
  }
  return new_x;
}

UNITS converty(UNITS y){
  UNITS new_y = y;

  if(y < 0){
    new_y = cols - 1;
  }
  else if(y >= cols){
    new_y = 0;
  }
  return new_y;
}

void mpNextGen(void){
  //printf("in method\n");
  #pragma omp parallel
    {
      int ID = omp_get_thread_num();
      int i,neigh;
      UNITS x,y;
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


void init(void){

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

int neighbours(UNITS x, UNITS y){
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

void freecells(void){
  printf("free\n");
  free(cells);
  free(temp);

}

void displaycells(void){
  UNITS i,j;
  for (i = 0; i < rows; i++) {
          //printf("%i: ", i);
          for (j = 0; j < cols; j++) {
            if(Verbose > 1 ){
              if (getcell(i,j) != 0 && getcell(i,j) != 1) {
                  fprintf(stderr, "there is a corruption at %li %li\n", i, j);
              }
              }
              
              printf("%i", getcell(i,j));
          }
          printf("\n");
      }
      printf("\n");
}

UNITS coord(UNITS x, UNITS y){
  return ((y)*cols+(x));
}
void setcellAlive(UNITS x,UNITS y){
  (cells[coord((x),(y))] = 1);
  //printf("cell x %i y %i is alive\n", x, y);
  //displaycells();
}
void setcellKill(UNITS x, UNITS y){
  (cells[coord((x),(y))] = 0);
  //printf("cell x %i y %i is dead\n", x, y);
  //sdisplaycells();
}

void setcellAliveT(UNITS x, UNITS y){
  (temp[coord((x),(y))] = 1);
}

void setcellKillT(UNITS x, UNITS y){
  (temp[coord((x),(y))] = 0);
}

void copyCells(void){
  memcpy(temp, cells, SIZE);
}

void copyTemp(void){
  //printf("hello\n");
  memcpy(cells,temp,SIZE);
}


int getcell(UNITS x, UNITS y){
  return (cells[coord(x,y)]);
}

int checkAlive(UNITS x, UNITS y){
  if(getcell(x,y) == 1){
    return true;
  }
  else{
    return false;
  }
}

void load_file(char* file, UNITS x, UNITS y){
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
    UNITS curx = x;
    UNITS cury = y;

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