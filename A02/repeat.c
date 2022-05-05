#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char s[100];
  int n;
  printf("Enter a word: ");
  scanf("%s",s);
  printf("Enter a count: ");
  scanf("%d",&n);
  
  int sizeIString = strlen(s);
  int sizeOString = sizeIString * n;
  char* output = (char*)malloc(sizeof(char)*sizeOString + 1);
  if(output == NULL){
    printf("Cannot allocate new string. Exciting...\n");
    exit(0);
  }
  output[0] = 0;
  //output = "";
  for(int i = 0; i < n; i++){
    strcat(output, s);
  }
  printf("Your word is %s\n", output);
  free(output);
  output = NULL;
  return 0;
}
