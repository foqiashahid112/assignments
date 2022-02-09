#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
  srand(time(0));
  FILE* infile;
  infile = fopen("words.txt","r");
  if(infile == NULL){
    printf("Error: unable to open file %s\n", "input.txt");
    exit(1);
  }

  char buffer[33];
  fgets(buffer, 33, infile);
  int linesSize = atoi(buffer);
  int random = rand() % linesSize;
  for(int i = 0; i < random; i++){
    fgets(buffer, 33, infile);
  }
  //printf("word is %s", buffer);
  
  printf("Welcome to Word Guess!\n");
  printf("\n\n");
  int n = strlen(buffer) - 1;
  char* guessing = (char*)malloc(sizeof(char)*n);
  for(int i = 0; i < n; i++){
    guessing[i] = '_';
  }
  guessing[n] = '\0';
  bool complete = false;
  int found = 0;
  int turns = 1;
  while(!complete){
    printf("Turn: %d\n", turns);
    printf("\n\n");
    turns++;
    for(int i = 0; i < n; i++){
      printf("%c ", guessing[i]);
    }
    printf("\n"); 
    printf("Guess a character: ");
    char guessed;
    scanf(" %c", &guessed);
    bool thisFound = false;
    for(int j = 0; j < n; j++){
      if(buffer[j] == guessed && guessing[j] != guessed){
        guessing[j] = guessed;
        found++;
        thisFound = true;
      }
    }
    if(thisFound == false){
      printf("Sorry, %c not found!\n", guessed);
    }
    if(found == n){
         complete = !complete;
         turns--;
    }
     
 }
  printf("\n");
  for(int i = 0; i < n; i++){
    printf("%c ", guessing[i]);
  }
  printf("\n");
  printf("You won in %d turns!\n", turns);
  free(guessing);
  return 0;
}
