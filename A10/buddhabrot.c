#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <stdbool.h>
#include <math.h>


struct thread_data{
	int id;
	int size;
	struct ppm_pixel* palette_colors;
	struct ppm_pixel* array_pixels;
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	int maxIterations;
	int start_R;
	int end_R;
	int start_C;
	int end_C;
  bool* escapes;
  int* count;
  int maxCount;
};

pthread_mutex_t mutex;
static pthread_barrier_t barrier; 

void *find_image(void *userdata){
	struct thread_data *data = (struct thread_data *) userdata;
  int id = data->id;
	int size = data->size;
  struct ppm_pixel* array_pixels = data->array_pixels;
  struct ppm_pixel* palette_colors = data->palette_colors;
  int* count = data->count;
  bool* escapes = data->escapes;
  float xmin = data->xmin;
	float xmax = data->xmax;
	float ymin = data->ymin;
	float ymax = data->ymax;
	int maxIterations = data->maxIterations;
	int start_R = data->start_R;
	int end_R = data->end_R;
	int start_C = data->start_C;
	int end_C = data->end_C;
  int maxCount = data->maxCount;


	pthread_mutex_lock(&mutex);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", data->id, data->start_R, data->end_R, data->start_C,data->end_C);
	 //Step 1:
   for(int i = start_R ; i < end_R; i++){
	    for(int j = start_C ; j < end_C; j++){
	      float xfrac = (float) i / (float) size;
	      float yfrac = (float) j / (float) size;
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

	      if(iter < maxIterations){
          escapes[j*size + i] = false;
	      }else{
          escapes[j*size + i] = true;
	      }
	    }
    }

  //Step 2: Compute visited counts
  for(int i = start_R ; i < end_R; i++){
	  for(int j = start_C ; j < end_C; j++){
      //if (row,col) belongs to the mandelbrot set, continue
      if(escapes[j* size + i] == true) continue;
	      float xfrac = (float) i / (float) size;
	      float yfrac = (float) j / (float) size;
	      float x_0 = xmin + xfrac * (xmax - xmin);
	      float y_0 = ymin + yfrac * (ymax - ymin);

	      float x = 0;
	      float y = 0;
	      int iter = 0;
	      while(iter < maxIterations && x*x + y*y < 2*2){
          float xtmp = x*x - y*y + x_0;
          y = 2*x*y + y_0;
          x = xtmp;
          int yrow = round(size * (y - ymin)/(ymax - ymin));
          int xcol = round(size * (x - xmin)/(xmax - xmin));
          if (yrow < 0 || yrow >= size) continue; // out of range
          if (xcol < 0 || xcol >= size) continue; // out of range

          //increment count at (yrow, xcol)
          count[j*size + i] += 1;
          //update max count
          if(count[j*size + i] > maxCount) maxCount = count[j*size + i];
	      }
	  }
  }

  pthread_barrier_wait(&barrier);

  float gamma = 0.681;
  float factor = 1.0/gamma;
  for(int i = start_R ; i < end_R; i++){
	  for(int j = start_C ; j < end_C; j++){
      int value = 0;

      if(count[j*size + i] > 0){
        value = log(count[j*size + i]) / log(maxCount);
        value = pow(value, factor);
      }
      struct ppm_pixel color;
      color.red = value * 225;
      color.green = value * 255;
      color.blue = value * 255;
      array_pixels[j*size + i] = color;
	  }
  }


  

	printf("Thread %d) finished\n",id);	
	pthread_mutex_unlock(&mutex);
	return NULL;
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

  bool* escapes;
  escapes = (bool *) malloc(size * size * sizeof(bool));

  int* count;
  count = (int *) malloc(size*size*sizeof(int));

  for(int i = 0 ; i< (size*size); i++){
    count[i] = 0;
  }

  int maxCount = 0;
  // generate pallet
  struct ppm_pixel* palette_colors;
  palette_colors = (struct ppm_pixel*) malloc(maxIterations * sizeof(struct ppm_pixel));

  //generate palette
  for(int i = 0; i < maxIterations; i++){
    palette_colors[i].red = rand() % 255;
    palette_colors[i].green = rand() % 255;
    palette_colors[i].blue = rand() % 255;
  }
  printf("Palette initialized\n");
  
  struct ppm_pixel* array_pixels; 
  array_pixels = (struct ppm_pixel*) malloc(size*size* sizeof(struct ppm_pixel));
  gettimeofday(&tstart, NULL);  
  // compute image using 4 threads
  pthread_t threads[4];
  struct thread_data data[4];
  int subsize = size / 4;

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 4);

  for(int i = 0; i < 4; i++){
    printf("This is loop number %d\n", i);
    data[i].escapes = escapes;
    data[i].count = count;
    data[i].id = i;
    data[i].size = size;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    if(i == 0){
      data[i].start_R = 0;
      data[i].end_R = size/2;
      data[i].start_C = 0;
      data[i].end_C = size/2;
    } else if(i == 1){
      data[i].start_R = size/2;
      data[i].end_R = size;
      data[i].start_C = 0;
      data[i].end_C = size/2;
    } else if (i == 2){
      data[i].start_R = 0;
      data[i].end_R = size/2;
      data[i].start_C = size/2;
      data[i].end_C = size;
    }else if (i == 3){
      data[i].start_R = size/2;
      data[i].end_R = size;
      data[i].start_C = size/2;
      data[i].end_C = size;
    }
    data[i].palette_colors = palette_colors;
    data[i].array_pixels = array_pixels;
    data[i].count = count;
    data[i].maxCount = maxCount;
    pthread_create(&threads[i], NULL, find_image, (void*) &data[i]);
  }
  
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }


  char outputFile[1000];
  sprintf(outputFile, "buddhabrot-%d-%ld.ppm", size,time(0));

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&mutex); 
  
  write_ppm(outputFile, array_pixels, size, size);
  
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  printf("Computed mandelbrot set (%dx%d) in %g\n", size, size, timer);
  printf("Writing file: %s\n", outputFile);


  free(palette_colors);
  palette_colors = NULL;
  free(array_pixels);
  array_pixels = NULL;
  
  return 0;
}
