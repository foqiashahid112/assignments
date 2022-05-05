//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name:
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* newNode = malloc(sizeof(struct node));
  if(newNode == NULL){
    printf("Error: Out of space!\n");
    exit(1);
  }
  newNode->sym = sym;
  newNode->linenum = line;
  newNode->colnum = col;
  newNode->next = top;
  top = newNode;
  return top;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  if(top == NULL){
    return NULL;
  }
  struct node* temp = top;
  top = top->next;
  temp->next = NULL;
  free(temp);
  temp = NULL;
  return top; 
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  if(top == NULL){
    free(top);
    return;
  }
  struct node* temp = top;  
  while(top != NULL){
    temp = top;
    top = top->next;
    temp->next = NULL;
    free(temp);
    temp = NULL; 
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  if(top == NULL){
    printf("Empty \n");
    return;
  }
  struct node* temp = top;
  while(temp!= NULL){
    printf("%c, %d, %d \n", temp->sym, temp->linenum, temp->colnum);
    temp = temp->next;
  }
  free(temp);
  temp = NULL;
}

int main(int argc, char* argv[]) {
  if(argc != 2){
    printf("usage: %s \n", argv[0]);
    exit(1);
  }
  FILE* infile;
  infile = fopen(argv[1], "r");
  if(infile == NULL){
    printf("Cannot open file: %s\n", argv[1]);
    exit(0);
  }
  struct node* stack = NULL;
  int ch;
  int r = 1;
  int c = 0;
  ch = getc(infile);
  while(ch != EOF){
    ch = fgetc(infile);
    if(ch == '{'){
      stack = push(ch , r, c, stack);
    }
    if(ch == '}'){
      if(stack == NULL){
        printf("Unmatched brace on Line %d and Column %d \n" , r , c);
      }else{
        printf("Found matching brackes:(%d, %d) -> (%d, %d)\n", stack->linenum, stack->colnum, r, c);
        stack = pop(stack);
      }
    }
    if(ch == '\n'){
      r += 1;
      c = 0;
    } 
    c+=1;
  }
  fclose(infile);
  while(stack != NULL){
    printf("Unmatched brace on Line %d and Column %d \n", stack->linenum, stack->colnum);
    stack = pop(stack);
  }
  return 0;
}
