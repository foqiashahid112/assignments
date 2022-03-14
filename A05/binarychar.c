#include <stdio.h>



int binaryToDecimal(char* inputBinary){
  int decimal = 0;
  int multiple = 1; //pow(2,0)
  for(int i = 7; i >= 0; i--){
    if(inputBinary[i] == '1') decimal += multiple;
    multiple = multiple * 2;
  }
  return decimal;
}


int main() {
  char input[8];
  printf("Enter 8 bits: ");
  scanf("%s", input);
  int decimal = binaryToDecimal(input);
  printf("binary to decimal %d", decimal);
  //printf("Your character is: %c\n", character);
  return 0;
}
