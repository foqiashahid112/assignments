//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name : Foqia
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};


// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* snacks, 
  const char* name, int quantity, float cost) {
    
  struct snack* newSnack = malloc(sizeof(struct snack));
  if(newSnack == NULL){
    printf("ERROR: Out of Space!\n");
    exit(1);
  }
  strcpy(newSnack->name , name);
  newSnack->quantity = quantity;
  newSnack->cost = cost;
  newSnack->next = NULL;
  
  if(snacks == NULL){
    return newSnack;
  }
  else if(strcmp(name,snacks->name) < 0){
  	newSnack->next = snacks;
	snacks = newSnack;
	free(newSnack);
	newSnack = NULL;
	return snacks;
  }
  else{
	struct snack* temp = snacks;
  	while(temp->next != NULL){
		if(strcmp(name, temp->next->name) < 0) break;
		temp = temp->next;
	}
	struct snack* new = NULL;
	new = (struct snack*)malloc(sizeof(struct snack));
	strcpy(new-> name,name);
	new->quantity = quantity;
	new->cost = cost;
	new->next = temp->next;
	temp->next = new;
	free(newSnack);
	newSnack = NULL;
	return snacks;
  
  }

}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  struct snack* head = snacks;
  struct snack* temp = snacks;
  while(temp != NULL){
    temp = temp->next;
    free(head);
    head = temp;
  }
}

int main() {
  int number;
  printf("Enter a number of snacks: ");
  scanf("%d", &number);
  struct snack* snackbar = NULL;

  for(int i = 0; i < number; i++){
    char name[100];
    float cost = 0;
    int quantity = 0;
    printf("Enter a name: ");
    scanf("%s", name);
    printf("Enter a cost: ");
    scanf("%f", &cost);
    printf("Enter a quantity: ");
    scanf("%d", &quantity);
    snackbar = insert_sorted(snackbar, name, quantity, cost);
  }
  struct snack* head = snackbar;

  printf("Welcome to Sorted Sally's Snack Bar.\n");
  printf("\n");
  for(int i = 0; i < number; i++){
    printf("%d) %s   cost: $%.2f   quantity: %d \n", i, snackbar->name, snackbar->cost, snackbar->quantity);
    snackbar = snackbar->next;
    if(snackbar == NULL) break;
  }
  clear(head);
  return 0;
}

