#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"
#define LOOPS 10
int main (int argc, char* argv[]) {
  int size = 100;

  void *init = sbrk(0);
  void *current;

  printf("The intial top of the heap is %p. \n", init);

  for(int i = 0; i < LOOPS; i++){
  	int *memory;
	memory = malloc(size);
	if( memory == NULL){
		fprintf(stderr, "malloc failed\n");
		return (1);
	}
	*memory = 123;
	free(memory);
  }
  current = sbrk(0);
  printf("The current top of the heap is %p. \n", current);
  int allocated = (int) (current - init);
  printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
  return 0 ;
}
