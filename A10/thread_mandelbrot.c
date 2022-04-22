#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"

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
};

pthread_mutex_t mutex;

void *find_image(void *userdata){
	struct thread_data *data = (struct thread_data *) userdata;
	//pthread_mutex_lock(&mutex);
	printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d,%d)\n", data->id, data->start_R, data->end_R, data->start_C,data->end_C);
	 for(int i = data->start_R ; i < data->end_R; i++){
	    for(int j = data->start_C ; j < data->end_C; j++){
	      float xfrac = (float) j / (float) data->size;
	      float yfrac = (float) i / (float) data->size;
	      float x_0 = data->xmin + xfrac * (data->xmax - data->xmin);
	      float y_0 = data->ymin + yfrac * (data->ymax - data->ymin);

	      float x = 0;
	      float y = 0;
	      int iter = 0;
	      while(iter < data->maxIterations && x*x + y*y < 2*2){
		float xtmp = x*x - y*y + x_0;
		y = 2*x*y + y_0;
		x = xtmp;
		iter++;
	      }
	      struct ppm_pixel color;
	      if(iter < data->maxIterations){
		color.red = data->palette_colors[iter].red;
		color.blue = data->palette_colors[iter].blue;
		color.green = data->palette_colors[iter].green;
	      }else{
		color.red = 0;
		color.blue = 0;
		color.green = 0;
	      }
	      //write color to image at location (row, col)
	      pthread_mutex_lock(&mutex);
	      data->array_pixels[i*data->size + j] = color;
	      pthread_mutex_unlock(&mutex);
	    }
	 }
	printf("Thread %d) finished\n",data->id);	
	//pthread_mutex_unlock(&mutex);
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
  for(int i = 0; i < 4; i++){
	printf("This is loop number %d\n", i);
	data[i].id = i;
 	data[i].size = subsize;
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

	} else if (i == 3){
	data[i].start_R = size/2;
	data[i].end_R = size;
	data[i].start_C = size/2;
	data[i].end_C = size;

	}
	data[i].palette_colors = palette_colors;
	data[i].array_pixels = array_pixels;
	pthread_create(&threads[i], NULL, find_image, (void*) &data[i]);
  }
  
  pthread_mutex_destroy(&mutex); 
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  char outputFile[1000];
  sprintf(outputFile, "mandelbrot-%d-%ld.ppm", size,time(0));
  write_ppm(outputFile, array_pixels, size, size);
  
  printf("Computed mandelbrot set (%dx%d) in %g\n", size, size, timer);
  printf("Writing file: %s\n", outputFile);

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);

  }

  free(palette_colors);
  palette_colors = NULL;
  free(array_pixels);
  array_pixels = NULL;
  
  return 0;

}
