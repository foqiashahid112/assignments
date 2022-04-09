#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
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
  
  struct ppm_pixel** array_pixels;
  array_pixels = (struct ppm_pixel**) malloc (size * sizeof(struct ppm_pixel*));
  for(int i = 0; i < size; i++){
    array_pixels[i] = (struct ppm_pixel*)  malloc (size * sizeof(struct ppm_pixel));
  }  
  
  gettimeofday(&tstart, NULL);
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      float xfrac = (float) i / (float) size;
      float yfrac = (float) j / (float) size;
      float x_0 = xmin + xfrac * (xmax - xmin);
      float y_0 = ymin + yfrac * (ymax - ymin);
      
      int x = 0;
      int y = 0;
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
      array_pixels[i][j] = color;
    } 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g\n", size, size, timer);
 
  //write to file
  char outputFile[1000];
  sprintf(outputFile, "mandelbrot-%d-%ld.ppm", size,time(0));
  write_ppm(outputFile, array_pixels, size, size);
  printf("Writing file: %s\n", outputFile);
 
  //Free space
  for(int i = 0; i < size; i++){
    free(array_pixels[i]);
  }
  free(array_pixels);
  free(palette_colors);
  array_pixels = NULL;
  palette_colors = NULL;
  return 0;
}
