#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
struct chunk {
  int size;
  struct chunk *next;
  int inUse;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  
  struct chunk *next = flist ;
  struct chunk *prev = NULL;
  while(next != NULL) {
	if (next->size >= size) {
			if(prev != NULL) {
			prev->next = next->next;
		} else {
			flist = next->next;
		}
		next->inUse= size;
		return (void*)(next + 1);
	} else {
		prev = next;
		next = next->next;
	}
	}


  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size;
    return (void*) (cnk + 1);
  }
}

void free(void *memory) {
  if(memory != NULL){
	  struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
	  cnk->next = flist;
	  flist = cnk;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  struct chunk *next = flist;
  int freeChunks = 0; //in flist
  int free_largest = 0;
  int free_smallest = INT_MAX;
  float free_average = 0;
  while(next != NULL){
     freeChunks++;
     int tempFree = next->size;
     free_average += tempFree;
     free_average += tempFree; 
     if(tempFree > free_largest) free_largest = tempFree;
     if(tempFree < free_smallest) free_smallest = tempFree;
     next = next->next;
  }
  int usedChunks = 0;//stored in buffer
  int unused_largest = 0;//largest unused memory across all used chunks
  int unused_smallest = INT_MAX;//smallest unused memory across all used chunks
  float unused_average = 0;//average unused memory across all used chunks
  for(int i = 0; i < len; i++){
     if(buffers[i] != NULL){
     	usedChunks++;
	struct chunk *cnk = (struct chunk*)((struct chunk*) buffers[i] - 1 );
	struct chunk *cnk = (struct chunk*)((struct chunk*) buffers[i] - 1 );	
	int tempSize = cnk->size;//temp stores the size associated with this used memory chunk
	int tempInUse = cnk->inUse;
	int tempUnUsed = tempSize - tempInUse;
	unused_average += tempUnUsed;
	if(tempUnUsed < unused_smallest) unused_smallest = tempUnUsed;
	if(tempUnUsed > unused_largest) unused_largest = tempUnUsed;
     }
  }
  int total = freeChunks+usedChunks;
  int internal = unused_average;
  int external = free_average;
  free_average = free_average / freeChunks;
  unused_average = unused_average / usedChunks;
  printf("Total blocks: %d, Free: %d, Used: %d\n", total, freeChunks, usedChunks);
  printf("Internal unused: total: %d average: %0.1f smallest: %d largest: %d\n", internal , unused_average, unused_smallest, unused_largest);
  printf("Internal unused: total: %d average: %0.1f smallest: %d largest: %d\n", internal , unused_average, unused_smallest, unused_largest);  
  printf("External unused: total: %d average: %0.1f smallest: %d largest: %d\n", external, free_average, free_smallest, free_largest);
}
