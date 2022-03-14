#include <stdio.h>

int main() {
  char input[5];
  printf("Enter 4 characters: " );
  scanf("%s", input);
  printf("%s", input);
  printf("\n");  
  int shiftFactor = 0;
  unsigned int hexForm = 0x00000000;
  for(int i = 3; i >= 0; i--){
    unsigned int toPrint = input[i];
    unsigned int shifted = toPrint << shiftFactor;   
    printf("Debug: %c = 0x%08x\n", input[i], shifted);
    shiftFactor += 8;
    hexForm = hexForm | shifted;
    
  }
  
  printf("Your number is: %d (0x%x)\n", hexForm, hexForm);
  return 0;
}

