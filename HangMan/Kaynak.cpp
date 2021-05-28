#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h> 
#include<time.h> 

int isUserPlayBefore(int id) {
	FILE* f;
	f = fopen("saves.txt", "r");

	int oldid, oldwin, oldplayCount, oldmaxLevel;

	while (fscanf(f, "%d %d %d %d", &oldid, &oldwin, &oldplayCount, &oldmaxLevel) != EOF) {
		if (id == oldid)
			return 1;
	}
	return 0;
}

void  createnewplayer(int id, int numWin, int playCount, int maxLevel) {
	FILE* f;
	f = fopen("saves.txt", "a");
	fprintf(f, "\n%d %d %d %d\n", id, numWin, playCount, maxLevel);
	fclose(f);
}

int   TheBigOne(int num1, int num2) {
	if (num1 > num2)
		return num1;
	return num2;
}

void  updateUser(int id, int numWin, int playCount, int maxLevel) {
	FILE* f = fopen("saves.txt", "r+");
	int fid, fnumWin, fplayCount, fmaxLevel;
	while (!feof(f)) {
		int whereIsCursor = ftell(f);
		fscanf(f, "%d %d %d %d", &fid, &fnumWin, &fplayCount, &fmaxLevel);
		if (fid == id) {
			fseek(f, whereIsCursor, SEEK_SET);
			fprintf(f, "\n%d %d %d %d\n", id, fnumWin + numWin, fplayCount + playCount, TheBigOne(maxLevel, fmaxLevel));
			break;
		}
	}
	fclose(f);
}

void  userOperations(int id, int numWin, int playCount, int level) {
	if (isUserPlayBefore(id))
		updateUser(id, numWin, playCount, level);
	else
		createnewplayer(id, numWin, playCount, level);
}

void report(int id) {

	FILE* olddatas;
	olddatas = fopen("saves.txt", "r");

	int oldid, oldwin, oldplays, oldmaxlvl;
	while (fscanf(olddatas, "%d %d %d %d", &oldid, &oldwin, &oldplays, &oldmaxlvl) != EOF) {
		if (id == oldid) {
			printf("\nID is: %d\n", oldid);
			printf("Number of plays: %d\n", oldplays);
			printf("Number of wins: %d\n", oldwin);
			printf("The max level: %d\n", oldmaxlvl);
		}
	}
	fclose(olddatas);

}

//-----------------------------------------------------------------------------------------------//

int numgenerator(int lensize) {
	int num;
	if (lensize == 3) {
		srand(time(NULL));
		num = rand() % 999 + 100;
	}
	if (lensize == 4) {
		srand(time(NULL));
		num = rand() % 9999 + 1000;
	}
	if (lensize == 5) {
		srand(time(NULL));
		num = rand() % 99999 + 10000;
	}

	return num;
}

int  isItcorrect(char* arrStar, int lenArr) {
	int count = 0;
	for (int i = 0; i < lenArr; i++)
		if (arrStar[i] != '*') {
			count++;
			if (count == lenArr) {
				return 1;
			}
		}
	return 0;
}

char* starGenerator(int lenArr) {
	char* ptr;
	ptr = (char*)malloc(lenArr * sizeof(char));
	for (int i = 0; i < lenArr; i++)
		ptr[i] = '*';

	return ptr;
}

void  openStars(int userGuess, char* arrStar, int lenArr, int theNum) {
	char num = userGuess + '0';
	int index = lenArr - 1;
	while (theNum > 0) {
		if (userGuess == theNum % 10)
			arrStar[index] = num;
		index--;
		theNum /= 10;
	}
}

int   isExists(int userGuess, int theNum) {
	int temp = theNum;
	while (temp > 0) {
		if (userGuess == temp % 10)
			return 1;
		temp /= 10;
	}
	return 0;
}

int menu() {
	printf("\nEnter your choice:\n");
	printf("1.Start Game\n");
	printf("2.End Game\n");

	int choice1;
	scanf_s("%d", &choice1);
	if (choice1 > 7 || choice1 < 1)
		return menu();
	return choice1;

}

int   getID() {
	int id;
	printf("Please type your id number: ");
	scanf_s("%d", &id);
	if (id < 1000) {
		printf("ID num should be at least 4 digit and positive!\n");
		return getID();
	}
	return id;
}

void  hangman(int id) {
	int wins = 0, plays = 0;
	int numsize = 3, hak = 7, level = 1,maxlevel =1;
	while (level <= 3) {
		printf("\n---------- LEVEL %d ----------\n", level);
		int randNum = numgenerator(numsize);
		char* arrStar = starGenerator(numsize);
		int temp = hak; 
		while (temp > 0 && !isItcorrect(arrStar, numsize)){
			for (int i = 0; i < numsize; i++) {
				printf("%c", arrStar[i]);
			}
			printf("\n");
			int myguess;
			do{
				printf("Enter your guess: ");
				scanf_s("%d", &myguess);
				if (myguess > 10 || myguess < 0) {
					printf("You entered out of boundary!\n");
				}
			} while (myguess > 10 || myguess < 0);

			if (isExists(myguess, randNum))
				openStars(myguess, arrStar, numsize, randNum);

			temp--;
		}
		if ((isItcorrect(arrStar, numsize))) {
			level++; wins++; 
			numsize++; hak--;
			printf("You win the game! The generated number is %d", randNum);
		}
		else { 
			plays++;
			printf("You lost the game! The generated number was %d", randNum);
			break;
		}
	}
	userOperations(id, wins, plays, level);
	report(id);
}


int main() {
	int choice;
	
	do{
		choice = menu();
		switch (choice) {
		case 1:
			int id = getID();
			hangman(id);
			break;
		}

	} while (choice != 2);
	return 0;
}
