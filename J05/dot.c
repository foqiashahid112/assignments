#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

struct thread_data {
  int start_index;
  int end_index;
  int* v;
  int* u;
  int dot_product;
};

void *findDot(void *userdata){
  struct thread_data *data = (struct thread_data *) userdata;
  int dot_product = 0;
  for (int i = data->start_index; i < data->end_index; i++) {
    dot_product += data->v[i] * data->u[i];  
  }
  data->dot_product = dot_product;
  return NULL; 
}

int main(int argc, char *argv[]) {
  srand(time(0));
 
  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;
 
  for(int i = 0; i < SIZE; i++){
    v[i] = rand() % 1000 -500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  } 
  pthread_t threads[4];
  struct thread_data data[4];
  int subsize = SIZE / 4;
  for (int i = 0; i < 4; i++){
    data[i].v = v;
    data[i].u = u;
    data[i].start_index = subsize*i;
    data[i].end_index = subsize*(i+1);
    pthread_create(&threads[i], NULL, findDot, (void*) &data[i]);   
  } 

  int gDotproduct = 0;
  /* Join all the threads. Main will pause in this loop until all threads
   * have returned from the thread function. */
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
    gDotproduct += data[i].dot_product;
    
  }

 
  printf("Ground truth dot product: %d\n",dotproduct);
  printf("After threads dot product: %d\n", gDotproduct);
  // TODO: Implement your thread solution here
  printf("Test with 4 threads\n");

  return 0;
}
