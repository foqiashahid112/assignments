#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {
  //open file
  FILE* infile;
  infile  = fopen(filename, "r");
  if(infile == NULL){
    printf("Cannot open file: %s\n", filename);
    return NULL;
  }
  //read header
  char magic_number[3];
  char buffer[1000];
  //magic number
  fgets(buffer, 1000, infile);
  strcpy(buffer, magic_number);
  //white space
  fgets(buffer, 1000, infile);
  if(buffer[0] == '#'){
    fgets(buffer, 1000, infile);
    sscanf(buffer, "%d %d", w, h);
  }else{
    sscanf(buffer, "%d %d", w, h);
  }
  //maximum color value
  int maxColor;
  fgets(buffer, 1000, infile);
  sscanf(buffer, "%d", &maxColor);
  //malloc pixels
   struct ppm_pixel** array = malloc(sizeof(struct ppm_pixel*) * (*h));
  for(int i = 0; i < (*h); i++){
    array[i] = malloc(sizeof(struct ppm_pixel) * (*w));
  }
  if(array == NULL){
    printf("Malloc error\n");
    return NULL;
  }
  unsigned char r;
  unsigned char g;
  unsigned char b;
  for(int i = 0; i < *h; i++){
      for(int j = 0; j < *w; j++){
      fscanf(infile, " %hhu %hhu %hhu", &r, &g, &b);
      struct ppm_pixel new_pixel;
      new_pixel.red = r;
      new_pixel.green = g;
      new_pixel.blue = b;
      array[i][j] = new_pixel;
    }
  } 
  fclose(infile);
  return array; 
}

