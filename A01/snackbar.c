#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct snack{
	char name[100];
	int quantity;
	float cost;
};

int main() {
	struct snack cocopuffs;
	struct snack cheese;
	struct snack beans;

	/*cocopuff specifications*/
	strcpy(cocopuffs.name, "Coco Puffs");
	cocopuffs.quantity = 4;
	cocopuffs.cost = 1.5;

	/*cheese specifications*/
	strcpy(cheese.name, "Manchego cheese");
	cheese.quantity = 6;
	cheese.cost = 15.5;

	/*beans specification*/
	strcpy(beans.name, "Magic beans");
	beans.quantity = 0;
	beans.cost = 0.5;

	struct snack arr_snacks[3];
	arr_snacks[0] = cocopuffs;
	arr_snacks[1] = cheese;
	arr_snacks[2] = beans;

	printf("Welcome to Steven Struct's Snack Bar. \n");
	printf("How much money do you have? ");
	int money;
	scanf("%d", &money);

	for(int i = 0; i < 3; i++){
		printf("%d) %s   cost: $%.2f   quantity: %d\n",i,arr_snacks[i].name, arr_snacks[i].cost, arr_snacks[i].quantity);
	}
	

	int snackNumber;
	printf("What snack would you like to buy? [0,1,2] ");
	scanf("%d", &snackNumber);


	if(snackNumber == 0 || snackNumber == 1 || snackNumber == 2){
		float balance = money - arr_snacks[snackNumber].cost;
		bool snackAvailable = 1;
		if(balance < 0){
			printf("You can't afford it!\n");
		}else{
			if(arr_snacks[snackNumber].quantity <=  0){
				snackAvailable = 0;
			}
			

			if(snackAvailable){
				printf("You bought %s\n", arr_snacks[snackNumber].name);
				printf("You have $%.2f left\n", balance);
			}else{
				printf("Sorry, we are out of %s\n", arr_snacks[snackNumber].name);
			}
			
		}
	}else{
		printf("Invalid response!\n");
	}
   return 0;
}
