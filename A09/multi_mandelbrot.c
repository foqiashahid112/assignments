#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"

void computeMandelbrot(int size, int maxIterations, struct ppm_pixel* palette_colors, struct ppm_pixel* array_pixels, float xmin, float xmax, float ymin, float ymax, int start_R, int end_R,int start_C,int end_C){

  for(int i = start_R; i < end_R; i++){
    for(int j = start_C; j < end_C; j++){
      float xfrac = (float) j / (float) size;
      float yfrac = (float) i / (float) size;
      float x_0 = xmin + xfrac * (xmax - xmin);
      float y_0 = ymin + yfrac * (ymax - ymin);
      
      float x = 0;
      float y = 0;
      int iter = 0;
      while(iter < maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x_0;
        y = 2*x*y + y_0;
        x = xtmp;
        iter++;
      }
      struct ppm_pixel color;
      if(iter < maxIterations){
        color.red = palette_colors[iter].red;
        color.blue = palette_colors[iter].blue;
        color.green = palette_colors[iter].green;
      }else{
        color.red = 0;
        color.blue = 0;
        color.green = 0;
      }
      //write color to image at location (row, col)
      array_pixels[i*size + j] = color;
    } 
  }
}


int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  
  double timer;
  struct timeval tstart, tend;

  srand(time(0));
  
  struct ppm_pixel* palette_colors;
  palette_colors = (struct ppm_pixel*) malloc(maxIterations * sizeof(struct ppm_pixel));
  
  //generate palette 
  for(int i = 0; i < maxIterations; i++){
    palette_colors[i].red = rand() % 255;
    palette_colors[i].green = rand() % 255;
    palette_colors[i].blue = rand() % 255;   
  } 
 
  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* array_pixels = shmat(shmid, NULL, 0);
  if (array_pixels == (void*) -1) {                                              
    perror("Error: cannot access shared memory\n");                              
    exit(1);                                                                     
  }   
  
  gettimeofday(&tstart, NULL);  
  
  for(int i = 0; i < numProcesses; i++){
    int pid = fork();
    if(pid == 0){ //child
      if(i == 0){
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), 0, size/2, 0, size/2);      
        computeMandelbrot(size, maxIterations,palette_colors,array_pixels,xmin,xmax,ymin,ymax,0,size/2, 0, size/2);
	exit(0);
      }else if(i == 1){
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(),size/2,size,0,size/2);
        computeMandelbrot(size, maxIterations,palette_colors,array_pixels,xmin,xmax,ymin,ymax,size/2,size,0,size/2);
	exit(0);
      }else if(i == 2){
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), 0, size/2, size/2, size);
        computeMandelbrot(size, maxIterations,palette_colors,array_pixels,xmin,xmax,ymin,ymax,0,size/2,size/2,size);
	exit(0);
      }else if(i == 3){
      	printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(),size/2,size,size/2, size);
	      computeMandelbrot(size, maxIterations,palette_colors,array_pixels,xmin,xmax,ymin,ymax,size/2,size,size/2,size);
	      exit(0);
      }
   }else{
     printf("Lauched child process: %d\n", pid);
   }
  }

  for(int i = 0; i < numProcesses; i++){
  	int status;
	int pid = wait(&status);
	printf("Child process complete: %d\n", getpid());
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g\n", size, size, timer);

  char outputFile[1000];
  sprintf(outputFile, "mandelbrot-%d-%ld.ppm", size,time(0));
  write_ppm(outputFile, array_pixels, size, size);
  printf("Writing file: %s\n", outputFile);

  if (shmdt(array_pixels) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }   

  return 0;

}
