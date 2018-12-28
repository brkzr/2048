#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#define SIZE 4
#define PRINTC(c,f,s) printf ("\033[%dm" f "\033[0m", 30 + c, s)

enum Directions {UP,DOWN,LEFT,RIGHT,EXIT};

struct Board {
	int board[SIZE][SIZE];
};

int randomPoint(){
	return rand() % SIZE;
}

int randomValue(){
	return (rand() % 10) ? 2 : 4;
}

void printBoard(struct Board* b){
	int i,j;
	system("clear");
	printf("============================\n\r");
	for (j = SIZE - 1; j >= 0; --j) {
		printf("||");
		for (i = 0; i < SIZE; ++i) {
			if (b->board[i][j])
				//printf("%4d  ", b->board[i][j]);
				PRINTC ((int)log2(b->board[i][j]), "%4d  ", b->board[i][j]);
			else
				printf("      ");
		}
		printf("||\n\r");
	}
	printf("============================\n\r");
}

void transpose(struct Board* b){
	int i,j;
	struct Board b2;
	for (i = 0; i < SIZE; ++i)
		for (j = 0; j < SIZE; ++j)
			b2.board[i][j] = b->board[j][i];
	*b = b2;
}

void flip(struct Board* b){
	int i,j;
	struct Board b2;
	for (i = 0; i < SIZE; ++i)
		for (j = 0; j < SIZE; ++j)
			b2.board[i][j] = b->board[i][SIZE - j - 1];
	*b = b2;
}

void initBoard(struct Board* b){
	struct Board b2 = {};
	*b = b2;
	b->board[randomPoint()][randomPoint()] = randomValue();
	b->board[randomPoint()][randomPoint()] = randomValue();
}

void slideArray(int* a, int* b){
	int i,j=0,prev=0;
	for (i = 0; i < SIZE; ++i)
		if (a[i]) {
			if (a[i] == prev) {
				b[j - 1] *= 2;
				prev = 0;
			} else {
				b[j++] = a[i];
				prev = a[i];
			}
		}
}

bool isEqual(struct Board* a, struct Board* b){
	int i,j;
	for (i = 0; i < SIZE; ++i)
		for (j = 0; j < SIZE; ++j)
			if (a->board[i][j] != b->board[i][j])
				return false;
	return true;
}

bool mergeBoard(struct Board* b){
	int i; 
	struct Board b2 = *b;
	struct Board a = {};

	for (i = 0; i < SIZE; ++i)
		slideArray(b->board[i], a.board[i]);
	*b = a;

	if (isEqual(b, &b2))
		return false;
	return true;
}

void addRandomValue(struct Board* b){
	int i,j;
	while (1) {
		i = randomPoint();
		j = randomPoint();
		if (!b->board[i][j]) {
			b->board[i][j] = randomValue();
			return;
		}
	}
}

void move(struct Board* b, enum Directions direction){
	if (direction == LEFT || direction == RIGHT)
		transpose(b);
	if (direction == UP || direction == LEFT)
		flip(b);
	if (mergeBoard(b))
		addRandomValue(b);
    
	//REVERSE MATRIX CHANGES
	if (direction == UP || direction == LEFT)
		flip(b);
	if (direction == LEFT || direction == RIGHT)
		transpose(b);
}

enum Directions readMoveDirection(){
	int c;
	while (isspace(c = getchar()));
	if (c == EOF )
		return EXIT;
    switch(c){
        case 'w':
            return UP;
        case 'a':
            return RIGHT;
        case 's':
            return DOWN;
        case 'd':
            return LEFT;
        default:
            return EXIT;
    }
}

int main(){
	enum Directions c;
	struct Board b;
	initBoard(&b);
	printBoard(&b);
    system ("/bin/stty raw");
	while ((c = readMoveDirection()) != EXIT) {
		move(&b, c);
		printBoard(&b);
	}
   system ("/bin/stty cooked");
	return 0;
}