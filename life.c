#include <stdint.h>

#include "life.h"

static void *threadNextGen(void *);

pthread_barrier_t barrier;

#ifndef CELL
# define CELL uint8_t
#endif // CELL

//static unsigned long *cells = NULL;
static CELL *cells = NULL;
//static unsigned long *temp = NULL;
static CELL *temp = NULL;

int ticks = 1;
int loop = false;

int created = false;

long cols = 0;
long rows = 0;

int NUM_THREADS;

unsigned long CELLCOUNT;
unsigned long SIZE;
int Verbose = 0;

clock_t start, world_gen,file_time, end;
  
struct shapefile{
  char *fileName;
  int x;
  int y;
};

//int argc, char *argv[]

/*
  corruption happens in malloc
  final display does not have corruption but it also is inconsistent
    inconsistency is probs result of corruption.

*/


int main(int argc, char *argv[]){
  int opt;
  int file_loc = 0;
  int fx = 0, fy = 0;
  char *file = NULL;
  int j;
  struct shapefile filearray[10];
  start = clock();
  while((opt = getopt(argc, argv, "c:r:t:n:x:y:f:vl")) != -1 ){
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
        case 'l':
          loop = true;
          break;
          }
  }

  //printf("%i %i %i %i\n\n", cols, rows, ticks, NUM_THREADS);

  CELLCOUNT = ((long)cols * (long)rows);
  SIZE = (CELLCOUNT * sizeof(CELL));
  if(Verbose > 1){
    printf("cellcount %lu size %li %lu rows %li cols %li, sizeof cell %lu\n"
    , CELLCOUNT, (rows * cols) ,SIZE, rows, cols, (unsigned long) sizeof(CELL));
  }
  

    //displaycells();
    //testNeighbors();
    //testGen();

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
    //printf("serial code\n");
    //nextGeneration();
    //displaycells();
    
    

      /*
      printf("enter a file you wish to add: ");
      scanf(" %s %i %i", &file, &fx, &fy);
      printf("%i,%i,%s", fx,fy,file);
      */
      //load_file(file, fx, fy);
     // printf("do you wish to add another file? y/n: ");
     // scanf("%c", stop);
      
    /*
    for(int i = 0; i<= 10; i++){
      nextGeneration();
      printf("Gen #%i\n",i);
      //displaycells();
    }
    */
  //NUM_THREADS = 10; //atoi(argv[1]);
  if(loop){
    if(Verbose > 0){
      printf("in serial code\n");
    }
    nextGeneration();
  }
  else{
    {
      int i;
      int ids[NUM_THREADS];
      pthread_t threads[NUM_THREADS];

      if(Verbose > 0){
        printf("in thread code\n");
      }
      //thread_time = clock();
      memset(ids, 0, NUM_THREADS * sizeof(pthread_t));
      pthread_barrier_init(&barrier, NULL, NUM_THREADS);

      //  pthread_attr_init(&attr);
      //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

      for (i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, threadNextGen, &ids[i]);
      }
      for(i = 0; i < NUM_THREADS; i ++){
        pthread_join(threads[i],NULL);
      // printf("joined %i\n", i);
      }
      

      //printf("test\n");
      freecells();
      end = clock();
      printf("start\tworld gen\tfile place\tthread time\n");
      printf("%f\t%f\t%f\t%f\n",(((double)(end-start))/CLOCKS_PER_SEC), (((double)(world_gen-start))/CLOCKS_PER_SEC)
                                , (((double)(file_time-world_gen))/CLOCKS_PER_SEC), (((double)(end-file_time))/CLOCKS_PER_SEC));
      pthread_exit(EXIT_SUCCESS);
    }
  }
  freecells();
  end = clock();
  printf("start\tworld gen\tfile place\tthread time\n");
      printf("%f\t%f\t%f\t%f\n",(((double)(end-start))/CLOCKS_PER_SEC), (((double)(world_gen-start))/CLOCKS_PER_SEC)
                                , (((double)(file_time-world_gen))/CLOCKS_PER_SEC), (((double)(end-file_time))/CLOCKS_PER_SEC));
  return 0;
}

long coord(long x, long y){
  return ((y)*cols+(x));
}

void setcellAlive(long x,long y){
  (cells[coord((x),(y))] = 1);
  //printf("cell x %i y %i is alive\n", x, y);
  //displaycells();
}

void setcellKill(long x, long y){
  (cells[coord((x),(y))] = 0);
  //printf("cell x %i y %i is dead\n", x, y);
  //sdisplaycells();
}

void setcellAliveT(long x, long y){
  //(temp[coord((x),(y))] = 1);
  (temp[((y)*cols+(x))] = 1);
}

void setcellKillT(long x, long y){
 // (temp[coord((x),(y))] = 0);
  (temp[((y)*cols+(x))] = 0);
}

void copyCells(void){
  memcpy(temp, cells, SIZE);
}

void copyTemp(void){
  //printf("hello\n");
  memcpy(cells,temp,SIZE);
}

int getcell(long x, long y){
  return (cells[coord(x,y)]);
}

int checkAlive(long x, long y){
  if(getcell(x,y) == 1){
    return true;
  }
  else{
    return false;
  }
}

int getPopulation(void){
  int i,j;
  int population = 0;
  for (i = 0; i <= rows; i++){
    for(j = 0; j <= cols; j++){
      if(getcell(i,j)){
        population++;
      }
    }
  }
  return population;
}

void randcell(void){
  int i;
  for(i = 0; i<=1000; i++){
    int x = rand() % rows;
    int y = rand() % cols;
    setcellAlive(x,y);
  }
}

void displaycells(void){
  long i,j;
  for (j = 0; j < cols; j++)
    printf("%li", j%10);
  printf("\n");
  for (i = 0; i < rows; i++) {
          printf("%li: ", i);
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

void init(void){

  cells = malloc(SIZE);
  if(cells == NULL){
    printf("cells doesn't create %lu, cols %lu rows %lu cellcount %lu what size cals to %lu\n"
	   , SIZE, cols, rows, CELLCOUNT, (CELLCOUNT * sizeof(int)));
    exit(1);
  }
  memset(cells, 0, SIZE);

  temp = malloc(SIZE);
    if(temp == NULL){
      printf("temp doesn't create %lu\n", SIZE);
      exit(1);
    }      
  memset(temp, 0, SIZE);
 // printf("in init\n");
  //displaycells();
}

void freecells(void){
  //printf("free\n");
  free(cells);
  free(temp);

}

int neighbours(long x, long y){
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

void nextGeneration(void){
  long x,y,j;
  int neigh;

  for(j=0;j<ticks;j++){
    copyCells();
    for(x = 0; x < rows; x++){
      for(y = 0; y < cols; y++){
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
    if(Verbose > 0){
      displaycells();
    }
  }
}

void testGen(void){
  setcellAlive(2,4);
  setcellAlive(3,2);
  setcellAlive(3,4);
  setcellAlive(4,4);
  setcellAlive(4,3);
}

void testNeighbors(void){
  setcellAlive(3,3);
  setcellAlive(3,2);
  setcellAlive(3,4);

  printf("3,3, count %i\n",neighbours(3,3));
  printf("3,2, count %i\n",neighbours(3,2));
  printf("3,4, count %i\n",neighbours(3,4));
  displaycells();
}

void *threadNextGen(void *a){
  int id = *(int *) a;
  int i;
  //printf("start\n");

  for(i = 0; i < ticks; i++){
    if(id == 0){
      copyCells();
    }
   // printf("thread %i has arived first barrier\n", id);
    pthread_barrier_wait(&barrier);
    {long x;
    long y;
    int neigh;
    for(x = id; x < rows; x+= NUM_THREADS){
      for(y = 0; y < cols; y++){
        neigh = neighbours(x,y);
        if(neigh < 2 || neigh > 3){
          if(Verbose > 1){
          printf("%li,%li,kill\n",x,y);
          printf("its dead jim\n");
          }  
          setcellKillT(x,y);
        }
        if(neigh == 3){
          if(Verbose > 1){
          printf("%li,%li,alive\n",x,y);
          printf("its a live and set\n");
          }
          setcellAliveT(x,y);
        }
      }
    }
    pthread_barrier_wait(&barrier);

    if(id == 0){
      
      copyTemp();
      if(Verbose > 0){
        displaycells();
      }
      
    }
  }
  }
  //printf("ending now\n");
  //printf("thread %i has arived to the end\n", id);
  pthread_exit(EXIT_SUCCESS);
}

void load_file(char* file, long x, long y){
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
    long curx = x;
    long cury = y;

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
           // printf("thing\n");
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

long convertx(long x){
  long new_x = x;

  if(x < 0){
    new_x = rows - 1;
  }
  else if(x >= rows){
    new_x = 0;
  }
  return new_x;
}

long converty(long y){
  long new_y = y;

  if(y < 0){
    new_y = cols - 1;
  }
  else if(y >= cols){
    new_y = 0;
  }
  return new_y;
}
