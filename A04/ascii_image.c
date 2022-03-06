#include <stdlib.h>
#include <stdio.h>
#include "read_ppm.h"
#include <string.h>
char findSymbol(double intensity){
  if (intensity >= 0 && intensity < 26){
    return '@';
  }
  else if (intensity < 51){
    return '#';
  }
  else if (intensity < 76){
    return '%';
  } 
  else if (intensity < 101){
    return '*';
  }
  else if (intensity < 126){
    return 'o';
  }
  else if (intensity < 151){
    return ';';
  }
  else if (intensity < 176){
    return ':';
  }
  else if (intensity < 201){
    return ',';
  }
  else if (intensity < 226){
    return '.';
  }
  else if (intensity < 256){
    return ' ';
  }
  else {
    return'E';
  }
}

int main(int argc, char** argv) {
  char fileName[100];
  if(argc < 2){
    strcpy(fileName, "feep-ascii.ppm");
  } else{
    strcpy(fileName,argv[1]);
  }
  printf("filename is: %s\n",fileName);
  int w = 0;
  int h = 0;
  struct ppm_pixel** arrayPixels;
  arrayPixels = read_ppm(fileName, &w, &h);
  printf("Reading %s with width %d and height %d\n", fileName, w, h);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      struct ppm_pixel thisPixel = arrayPixels[i][j];
      unsigned char red = thisPixel.red;
      unsigned char green = thisPixel.green;
      unsigned char blue = thisPixel.blue;
      double I = (red + green + blue)/3;
      printf("%c", findSymbol(I));
    }
    printf("\n");
  }
  for(int i = 0; i < h; i++){
    free(arrayPixels[i]);
  }
  free(arrayPixels);
  arrayPixels = NULL;
  return 0;
}

