#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
int findWinner(char Pchoice[100], char AIchoice[100]){
	if(strcmp(Pchoice,AIchoice) == 0){
		return -1;
	}
	if(strcmp(Pchoice, "rock") == 0){
		if(strcmp(AIchoice, "scissors") == 0) return 1; //Player wins
		else return 0; //AI wins
	} else if(strcmp(Pchoice, "paper") == 0){
		if(strcmp(AIchoice, "rock") == 0) return 1;
		else return 0;
	} else{
		if(strcmp(AIchoice, "paper") == 0) return 1;
		else return 0;
	}
}

int main() {
  	printf("Welcome to Rock, Paper, Scissors!\n");
	printf("How many rounds do you want to play? ");
	int rounds;
	scanf("%d", &rounds);
	int AIScore = 0;
	int PScore = 0;
	for(int i = 0; i < rounds; i++){
		printf("Which do you choose? rock, paper, or scissors? ");
		char choice[100];
		char AIchoice[100];
		
		scanf("%s", choice);
		//AI chooses
		srand(time(0));
		int randchoice = rand() % 3;
		if(randchoice == 0) strcpy(AIchoice, "rock");
		else if(randchoice == 1) strcpy(AIchoice, "paper");
		else strcpy(AIchoice, "scissors");

		printf("AI chose %s\n", AIchoice);

		if(strcmp("rock", choice) == 0 || strcmp("paper", choice) == 0 || strcmp("scissors", choice) == 0){
			int winner = findWinner(choice, AIchoice);//0 - AI wins, 1 - Player wins, -1 tie
			if(winner == 1) PScore++;
			if(winner == 0) AIScore++;		
		} else{
			printf("You entered an invalid choice: %s\n", choice);
		}

		printf("AI score: %d, Player score: %d\n", AIScore, PScore);

	}

	if(AIScore > PScore){
		printf("AI wins!\n");
	} else if(AIScore < PScore){
		printf("Player wins!\n");
	} else{
		printf("Its a tie!\n");
	}
  return 0;
}

