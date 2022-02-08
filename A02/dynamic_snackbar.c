#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  int n;
  printf("Enter a number of snacks: ");
  scanf("%d",&n);
  //struct snack* allSnacks = malloc(sizeOf(struct snack)*n); //pointer to array
  struct snack allSnacks[n];
  for(int i = 0; i < n; i++){
    printf("Enter a name: ");
    scanf("%s", allSnacks[i].name);
    printf("Enter a cost: ");
    scanf("%f", &allSnacks[i].cost);
    printf("Enter a quantity: ");
    scanf("%d", &allSnacks[i].quantity);
  }

  printf("Welcome to Dynamic Donna's Snack Bar. \n");
  printf("\n");
  for(int i = 0; i < n; i++){
    printf("%d) %s      cost: $%f   quantity: %d \n", i, allSnacks[i].name, allSnacks[i].cost, allSnacks[i].quantity);
  } 
  return 0;
}
