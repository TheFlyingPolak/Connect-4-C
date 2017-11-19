/*	This program simulates the popular game Connect 4  */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#define clearScreen() system("cls")
#else
#define clearScreen() system("clear")
#endif

int move(char **board, int rows, int cols, int player);	// Facilitates each player's move
void printBoard(char **board, int rows, int cols);	// Prints the board to console
int scanBoard(char **board, int rows, int cols, short x, short y, int player);	// Checks if a winning situation is present

int main(void){
	int i, j, chipCount = 0;	// chipCount tracks the number of pieces on the board to detect when it is full
	bool game = true;			// bool game will be set to false to end the game in case of a win or a stalemate
	
	printf("Welcome to Connect 4, the classic vertical four-in-a-row checkers game!\n\n");
	
	/*	Ask players to select board size  */
	int choice;
	puts("Select board size:");
	puts("1: Classic   (6 x 7)");
	puts("2: Beefed up   (10 x 12)");
	puts("3: Humongous   (24 x 28)");
	scanf("%d", &choice);
	while (choice < 1 || choice > 3){
		puts("I'm sorry, we do not have such a board. Choose again.\n");
		puts("1: Classic   (6 x 7)");
		puts("2: Beefed up   (10 x 12)");
		puts("3: Humongous   (24 x 28)");
		scanf("%d", &choice);
	}
	
	/*	Generate board of selected size  */
	int rows, cols;
	switch (choice){
		case 1: rows = 6; cols = 7; break;		// Classic size
		case 2: rows = 10; cols = 12; break;	// Beefed up size
		case 3: rows = 24; cols = 28; break;	// Humongous size
	}
	char **board = malloc(rows * sizeof(char*));
	for (i = 0; i < rows; i++)
		board[i] = malloc(cols * sizeof(char));
	
	/*	Initialise board, set all slots to ' '  */
	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			board[i][j] = ' ';
		}
	}
	
	/*	Simulate the game  */
	while (game){
		/*	Player 1 move  */
		printBoard(board, rows, cols);
		if (move(board, rows, cols, 1) == 0){
			chipCount++;
		}
		else{
			printBoard(board, rows, cols);	// If move function detects a win, end the game
			puts("Player 1 wins!");
			game = false;
			break;
		}
		
		/*	Player 2 move  */
		printBoard(board, rows, cols);
		if (move(board, rows, cols, 2) == 0){
			chipCount++;
		}
		else{
			printBoard(board, rows, cols);	// If move function detects a win, end the game
			puts("Player 2 wins!");
			game = false;
			break;
		}
		
		/*	If the chip count reaches the limit of the board, end the game with a stalemate  */
		if (chipCount == rows * cols && game == true){
			printBoard(board, rows, cols);
			puts("The board is full! There is no winner!");
			game = false;
		}
	}
	
	/*	Ask the user to press ENTER key to exit  */
	char ch[2];
	puts("Press ENTER to exit");
	gets(ch);
	gets(ch);
	return 0;
}

/*	This function simulates each player's move and calls the scanBoard function  */
int move(char **board, int rows, int cols, int player){
	int i = 0, choice;
	bool loop = true;
	
	while (loop){
		/*	Ask player to select column to insert a chip  */
		printf("Player %d:\n", player);
		printf("Select column to insert chip, from 1 to %d: ", cols);
		scanf("%d", &choice);
		/*	Check if the number input is valid  */
		while (choice < 1 || choice > cols){
			puts("I'm sorry, we do not have that many columns. Choose again.\n");
			printf("Select column to insert chip, from 1 to %d: ", cols);
			scanf("%d", &choice);
		}
		/*	Check if the column selected is full  */
		if (board[0][choice - 1] != ' '){
			puts("This column is full! Choose again!\n");
		}
		else{
			loop = false;
		}
	}
	/*	Trace the column to determine where the chip is to be placed  */
	while (i != rows && board[i][choice - 1] == ' '){
		i++;
	}
	/*	Inset the chip  */
	switch (player){
		case 1: board[i - 1][choice - 1] = 'x'; break;
		case 2: board[i - 1][choice - 1] = 'o'; break;
	}
	
	/*	Call the scanBoard function to check whether the player has won  */
	return scanBoard(board, rows, cols, i - 1, choice - 1, player);
}

/*	This function prints the board to the console  */
void printBoard(char **board, int rows, int cols){
	int i, j, tmp;
	
	/*	Print all chips on the board and borders between columns  */
	clearScreen();
	for (i = 0; i < rows; i++){
		printf("%c", '|');
		for (j = 0; j < cols; j++){
			printf("%c%c", board[i][j], '|');
		}
		printf("\n");
	}
	/*	Print the bottom border of the board  */
	for (i = 0; i < cols; i++)
		printf("%c%c", '-', '-');
	printf("%c\n%c", '-', '|');
	
	/*	Print column numbers with borders  */
	for (i = 1; i <= cols; i++){
		tmp = i;
		while (tmp >= 10)
			tmp /= 10;
		printf("%d%c", tmp, '|');
	}
	printf("\n");
	/*	If there are 2 digit column numbers, print the second digit of each number below  */
	for (i = 1; i <= cols; i++){
		if (i >= 10)
			printf(" %d", i % 10);
		else
			printf("  ");
	}
	printf("\n\n");	
}

/*	This function attempts to detect a win around the last placed chip  */
int scanBoard(char **board, int rows, int cols, short x, short y, int player){
	int count, i, startRow, startCol;
	char c;
	
	switch(player){
		case 1: c = 'x'; break;
		case 2: c = 'o'; break;
	}
	count = 0;
	
	/*	Horizontal scan  */
	for (i = 0; i < cols; i++){
		if (board[x][i] == c){
			count++;
			if (count == 4)
				return 1;
			if (i != cols - 1){
				if (board[x][i + 1] != c)
					count = 0;
			}
		}
	}
	count = 0;
	/*	Vertical scan  */
	for (i = 0; i < rows; i++){
		if (board[i][y] == c){
			count++;
			if (count == 4)
				return 1;
			if (i != rows - 1){
				if (board[i + 1][y] != c)
					count = 0;
			}
		}
	}
	count = 0;
	/*	Diagonal scan left to right  */
	if (x == y){		// On diagonal
		startRow = 0;
		startCol = 0;
	}
	else if (y == 0 || x == 0){		// Top or left end of the board
		startRow = x;
		startCol = y;
	}
	else if (x > y){		// Lower triangle
		startRow = x - y;
		startCol = 0;
	}
	else if (x < y){		// Upper triangle
		startRow = 0;
		startCol = y - x;
	}
	
	while (startRow < rows && startCol < cols){	// Execute scan
		if (board[startRow][startCol] == c){
			count++;
			if (count == 4)
				return 1;
			if (startRow != rows - 1 && startCol != cols - 1){
				if (board[startRow + 1][startCol + 1] != c){
					count = 0;
				}
			}
		}
		startRow++;
		startCol++;
	}
	printf("\n");
	
	count = 0;
	/*	Diagonal scan right to left  */
	if (y == cols - x - 1){		// On diagonal
		startRow = 0;
		startCol = cols - 1;
	}
	else if (x == 0 || y == cols - 1){	// Top or right end of the board
		startRow = x;
		startCol = y;
	}
	else if (y > cols - x - 1){		// Lower triangle
		startRow = x - (cols - y - 1);
		startCol = cols - 1;
	}
	else if (y < cols - x - 1){		// Upper triangle
		startRow = 0;
		startCol = y + x;
	}
	
	while (startRow < rows && startCol >= 0){	// Execute scan
		if (board[startRow][startCol] == c){
			count++;
			if (count == 4)
				return 1;
			if (startRow != rows - 1 && startCol != 0){
				if (board[startRow + 1][startCol - 1] != c){
					count = 0;
				}
			}
		}
		startRow++;
		startCol--;
	}
	return 0;
}