#include "life.h"

static void *threadNextGen(void *);

static pthread_barrier_t barrier;

static int *cells = NULL;
static int *temp = NULL;

int ticks = 1;

int created = false;

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

  //printf("%i %i %i %i\n\n", cols, rows, ticks, NUM_THREADS);

  CELLCOUNT = (cols * rows);
  SIZE = (CELLCOUNT * sizeof(int));

    //displaycells();
    //testNeighbors();
    //testGen();
    if(!created){
      if(Verbose > 0){
        printf("creating world\n");
      }
      
      init();
      created = true;
    }
    for(j = 0; j < file_loc; j++){
      printf("loading file at location %i\n", j);
      load_file(filearray[j].fileName,filearray[j].x,filearray[j].y);
    }


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
  pthread_t threads[NUM_THREADS];
  int i;
  int ids[NUM_THREADS];

  pthread_attr_t attr;

  memset(ids, 0, NUM_THREADS * sizeof(pthread_t));
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for (i = 0; i < NUM_THREADS; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, threadNextGen, &ids[i]);
  }
  for(i = 0; i < NUM_THREADS; i ++){
    pthread_join(threads[i],NULL);
   // printf("joined %i\n", i);
  }

  //printf("test\n");
  //freecells();

  pthread_exit(EXIT_SUCCESS);

  //printf("hello world\n");
  return 0;
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

int getPopulation(){
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

void randcell(){
  int i;
  for(i = 0; i<=1000; i++){
    int x = rand() % rows;
    int y = rand() % cols;
    setcellAlive(x,y);
  }
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

void init(){

  cells = malloc(SIZE);
  if(cells == NULL){
    printf("cells doesn't create %i, cols %i rows %i cellcount %i what size cals to %i\n", SIZE, cols, rows, CELLCOUNT, (CELLCOUNT * sizeof(int)));
    exit(1);
  }
  memset(cells, 0, SIZE);

  temp = malloc(SIZE);
    if(temp == NULL){
      printf("temp doesn't create %i\n", SIZE);
      exit(1);
    }      
  memset(temp, 0, SIZE);
 // printf("in init\n");
  //displaycells();
}

void freecells(){
  printf("free\n");
  free(cells);
  free(temp);

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

void nextGeneration(){
  int x,y,neigh;

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
}

void testGen(){
  setcellAlive(2,4);
  setcellAlive(3,2);
  setcellAlive(3,4);
  setcellAlive(4,4);
  setcellAlive(4,3);
}

void testNeighbors(){
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
    int x,y,neigh;
    for(x = id; x < rows; x+= NUM_THREADS){
      for(y = 0; y < cols; y++){
        neigh = neighbours(x,y);
        if(neigh < 2 || neigh > 3){
          if(Verbose > 1){
          printf("%i,%i,kill\n",x,y);
          printf("its dead jim\n");
          }  
          setcellKillT(x,y);
        }
        if(neigh == 3){
          if(Verbose > 1){
          printf("%i,%i,alive\n",x,y);
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
  //printf("ending now\n");
  //printf("thread %i has arived to the end\n", id);
  pthread_exit(EXIT_SUCCESS);
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
