//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name:
//
#include <stdio.h>
#include <stdlib.h>

void isMagic(int** matrix, int rows, int columns){
  //first, lets check the rows
  int sum1 = 0;
  int sum2;
  for(int i = 0; i < columns; i++) sum1+= matrix[0][i];

  for(int i = 1; i < rows; i++){
    sum2 = 0;
    for(int j = 0; j < columns; j++){
      sum2+= matrix[i][j];
    }
    if(sum1 != sum2){
      printf("M is NOT a magic square!\n");
      return;
    }
  }

  //now, lets check columns
  for(int i = 0; i < columns; i++){
    sum2 = 0;
    for(int j = 0; j < rows; j++){
      sum2+= matrix[j][i];
    }
    if(sum1 != sum2){
      printf("M is not a magic square!\n");
      return;
    }
  }

  //now, lets check the diagnols
  sum2 = 0;
  for(int i = 0; i < rows; i++){
    sum2+= matrix[i][i];
  }
  if(sum1 != sum2){
    printf("M is not a magic square!\n");
    return;
  }
  sum2 = 0;
  for(int i = rows-1; i >= 0; i--){
    sum2+= matrix[i][i];
  }
  if(sum1 != sum2){
    printf("M is not a magic square!\n");
    return;
  }
  printf("M is a magic square (magic constant = %d)\n", sum1); 
}

int main() {
  int r, c;
  scanf("%d %d", &r, &c);

  int** matrix = malloc(sizeof(int*) * r);
  for(int i = 0; i < r; i++){
    matrix[i] = malloc(sizeof(int) * c);
  }
  
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      int val;
      scanf("%d", &val);
      matrix[i][j] = val;
      printf("%d ", matrix[i][j]);
      }
      printf("\n");
  }
  isMagic(matrix, r, c);
  
  for(int i = 0; i < r; i++){
    free(matrix[i]);
  }
  free(matrix);
  matrix = NULL;
  return 0;
}

