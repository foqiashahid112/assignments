#include <stdio.h>
#include <string.h>

int main() {
	printf("Enter a word: ");
	char word[100];
	scanf("%s",word);
	int length = strlen(word);
	for(int i = 0; i < length; i++){
		if(word[i] == 'e') word[i] = '3';
		if(word[i] == 'l') word[i] = '1';
		if(word[i] == 'a') word[i] = '@';
	}
	printf("Your bad password is %s\n", word);
	 return 0;
}
