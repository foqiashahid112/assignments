#include <stdio.h>
#include <string.h>
#include "read_ppm.h"
#include <stdlib.h>
int main(int argc, char** argv) {
  //Read input file
  if(argc != 2){
    printf("Error: input\n");
    return 0;
  }
  int w = 0;
  int h = 0;
  struct ppm_pixel** arrayPixels = read_ppm(argv[1], &w, &h);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  
  //Glitch File
  
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      arrayPixels[i][j].red = arrayPixels[i][j].red << (rand()%2);
      arrayPixels[i][j].green = arrayPixels[i][j].green << (rand()%2);
      arrayPixels[i][j].blue = arrayPixels[i][j].blue << (rand()%2);
    }
  }
  //Write a new file
  char outputFile[1000];
  strcpy(outputFile, argv[1]);
  strtok(outputFile, ".");
  strcat(outputFile, "-glitch.ppm");
  printf("Writing file %s\n", outputFile);
  
  write_ppm(outputFile, arrayPixels, w, h);
  //Free File
  for(int i = 0; i < h; i++){
    free(arrayPixels[i]);
  }
  free(arrayPixels);
  arrayPixels = NULL;
  return 0;
}
