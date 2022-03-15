#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>
int main(int argc, char** argv) {
  int w = 0;
  int h = 0;
  struct ppm_pixel** arrayPixels = read_ppm("feep-raw.ppm", &w, &h);
  printf("Testing file feep-raw.ppm: %d %d\n", w, h);
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      struct ppm_pixel this_pixel = arrayPixels[i][j];
      printf("(%d %d %d) ", this_pixel.red, this_pixel.green, this_pixel.blue);
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

