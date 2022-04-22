#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel** read_ppm(const char* filename, int* w, int* h) {

	FILE* infile;
  infile = fopen(filename, "rb");
  if(infile == NULL){
    printf("Cannot open file: %s\n", filename);
    return NULL;
  }
  //read header;
  char magic_number[100];
  char buffer[1000];
  //magic number;
  fgets(buffer, 1000,infile);
  strcpy(buffer, magic_number);
  //white space
  fgets(buffer, 1000,infile);
  if(buffer[0] == '#'){
    fgets(buffer, 1000, infile);
    sscanf(buffer, "%d %d", w, h);
  }else{
    sscanf(buffer, "%d %d", w, h);
  }
  //maximum color value
  int maxColor;
  fgets(buffer, 1000,infile);
  sscanf(buffer, "%d", &maxColor);
  //malloc pixels
  struct ppm_pixel** array = malloc(sizeof(struct ppm_pixel*) * (*h));
  for(int i = 0; i < (*h); i++){
    array[i] = malloc(sizeof(struct ppm_pixel) * (*w));
  }
  if(array == NULL){
    printf("Malloc error!\n");
    return NULL;
  }
  
  for(int i = 0; i < *h; i++){
    for(int j = 0; j < *w; j++){
      struct ppm_pixel new_pixel;
      fread(&new_pixel, sizeof(struct ppm_pixel), 1, infile);
      array[i][j] = new_pixel;
    }
  }
  fclose(infile);
  return array; 
  // your code here
}

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
FILE* infile;
  infile = fopen(filename, "wb");//w for write, b for binary
  //Add P6 magic number, blank space, width, height, max color valye, whitespace
  char* header;
  header = malloc(1000);
  fprintf(infile, "P6\n%d %d\n255\n", w,h);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      struct ppm_pixel new_pixel = pixels[(i*h) + j];
      fwrite(&new_pixel, sizeof(struct ppm_pixel), 1, infile);     
    } 
  }
  free(header);
  header = NULL; 
  fclose(infile);  
  // your code here
}

