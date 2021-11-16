#include "functions.h"

/*
Function name: welcome_screen
Description: This function displays the game menu. It asks for the user's input, and either starts the game,
prints the rules, or exits the program based on the user's input.
Input parameters: None
Returns: None
Preconditions: None
Postconditions: User needs to input a valid input (1, 2 or 3) or else the function loops
*/
void welcome_screen(void) {
	int input = 0;
	for (int i = 0; i == 0;) {
		printf("**** Welcome to Battleship! ****\n\n");
		printf("1. Print Rules\n2. Play Game\n3. Exit\nEnter what you'd like to do (1-3): ");
		scanf("%d", &input);
		if (input == 1) {
			i = 1;
			display_rules();
		}
		else if (input == 2) {
			i = 1;
			play_game();
		}
		else if (input == 3) {
			i = 1;
			printf("\nThank you for playing!\n\n");
		}
		else
			system("cls");
	}
}

/*
Function name: display_rules
Description: This function displays the rules of the game, then when the user is finished reading, returns to the welcome_screen
Input parameters: None
Returns: None
Preconditions: User entered 1 in welcome_screen
Postconditions: None
*/
void display_rules(void) {
	system("cls");
	printf("**** Rules of Battleship ****\n\n");
	printf("Ships:\nCarrier: 5 spaces long\nBattleship: 4 spaces long\nCrusier: 3 spaces long\nSubmarine: 3 spaces long\nDestroyer: 2 spaces long\n\n");
	printf("1. Battleship is a 2 player game. You are player 1 and the computer is player 2\n2. To begin, you place the 5 ships horizontally or vertically on a 10x10 board\n");
	printf("   (Ships can't overlap, and you'll choose to place them manually or randomly\n3. Once the ships are placed for both players, the game begins\n");
	printf("4. A player is chosen at random to begin\n5. The player selects coordinates within the 10x10 region of the enemy's board\n   to fire at\n6. The game then determines if the player hit a ship, or missed\n");
	printf("7. You will then see the results of each board (Neither player can see the \n   other's ships)\n8. After that it's the next player's turn (Steps 5-7 are repeated)\n9. If all spots on a ship are hit, the ship is sunk\n10. The first player to sink all of the opponents ships wins!\n");
	system("pause");
	system("cls");
	welcome_screen();
}

/*
Function name: play_game
Description: This function starts, and runs the game. Once the game is over, the program goes back to welcome_screen
Input parameters: None
Returns: None
Preconditions: User entered 2 in welcome_screen
Postconditions: Game needs to be played and finished
*/
void play_game(void) {
	srand((unsigned int)time(NULL));
	char p1[BORDER][BORDER], p2[BORDER][BORDER], p2_display[BORDER][BORDER]; //I made a board for p2, that p1 can see without seeing the ships
	int x_coord = 0, y_coord = 0, man_or_ran = 0, turn = 0, h_or_m = 0, p1_sunk = 0, p2_sunk = 0;
	Stats p1_stats = { 0,0,0,0.0 }, p2_stats = { 0,0,0,0.0 };
	FILE *outfile = NULL;
	outfile = fopen("Output.log", "w");
	init_board(p1);
	init_board(p2);
	init_board(p2_display);
	system("cls");
	for (int i = 0; i == 0;) {//This loop will check if the player entered 1 or 2, and will loop until they do so
		printf("Would you like to place your board 1. randomly or 2. manually?: ");
		scanf("%d", &man_or_ran);
		if (man_or_ran == 1) {
			i = 1;
			random_board(p1);
			printf("Your board: \n");
			print_board(p1);
		}
		else if (man_or_ran == 2) {
			i = 1;
			manual_board(p1);
		}
		else {
			printf("Invalid input, please enter your selection again.\n");
			system("pause");
			system("cls");
		}
	}
	system("cls");
	random_board(p2);
	turn = first_turn(); 
	printf("Player %d gets to go first!\n", turn + 1); //Determines which player gets to go first, and displays which player does (will just be the result of first_turn+1)
	while (p1_sunk < 5 && p2_sunk < 5) {//This loop will run until either player sinks 5 ships
		printf("Player 1's board:\n");
		print_board(p1);
		printf("Player 2's board:\n");
		print_board(p2_display);
		if (turn == 1) {//When turn is 1, it's player 2's turn
			for (int i = 0; i == 0;) {
				x_coord = rand() % 10;
				y_coord = rand() % 10;//Player 2 will randomly select coordinates, this is where I would implement something to make them have better AI
				if (p1[y_coord][x_coord] != 'm' && p1[y_coord][x_coord] != '*') {//Checks if the coordinates have been used already
					i = 1;
					printf("Player 2 fires at  %d %d,", x_coord, y_coord);
					if (hit_or_miss(p1, x_coord, y_coord) == 0) {//Checks if the player hit or missed, prints the result and updates the stats/log file
						printf(" and it's a miss!\n");
						fprintf(outfile, "Player 2: %d, %d miss\n", x_coord, y_coord);
						p2_stats.misses += 1;
						p2_stats.shots += 1;
						p1[y_coord][x_coord] = 'm';
					}
					else {
						printf(" and it's a hit!\n");
						fprintf(outfile, "Player 2: %d, %d hit", x_coord, y_coord);
						p2_stats.hits += 1;
						p2_stats.shots += 1;
						if (ship_sunk(p1, x_coord, y_coord, p2_sunk, outfile) == p2_sunk + 1)
							p2_sunk += 1;
						else {
							fprintf(outfile, "\n");
							printf("No ships were sunk.\n");
						}
					}
				}
			}
			system("pause");
			system("cls");
			turn--;//Turn is set to 0 so it's Player 1's turn
		}
		else if (turn == 0) {
			for (int i = 0; i == 0;) {
				printf("What coordinates would you like to fire at? (X Y): ");
				scanf("%d %d", &x_coord, &y_coord);
				if (x_coord >= 0 && x_coord <= 9 && y_coord >= 0 && y_coord <= 9) {//Checks that the coordinates are within the border
					if (p2[y_coord][x_coord] != 'm' && p2[y_coord][x_coord] != '*') {//Checks that the coordinates haven't been used
						i = 1;
						printf("Player 1 fires at  %d %d,", x_coord, y_coord);
						if (hit_or_miss(p2, x_coord, y_coord) == 0) {//Checks if the player hit or missed, prints the result and updates the stats/log file
							printf(" and it's a miss!\n");
							fprintf(outfile, "Player 1: %d, %d miss\n", x_coord, y_coord);
							p1_stats.misses += 1;
							p1_stats.shots += 1;
							p2[y_coord][x_coord] = 'm';
							p2_display[y_coord][x_coord] = 'm';
						}
						else {
							printf(" and it's a hit!\n");
							fprintf(outfile, "Player 1: %d, %d hit", x_coord, y_coord);
							p1_stats.hits += 1;
							p1_stats.shots += 1;
							p2_display[y_coord][x_coord] = '*';
							if (ship_sunk(p2, x_coord, y_coord, p2_sunk, outfile) == p2_sunk + 1)
								p1_sunk += 1;
							else {
								fprintf(outfile, "\n");
								printf("No ships were sunk.\n");
							}
						}
					}
					else {//Makes the player select coordinates again if they've already shot there
						printf("You've already fired at that location, try again!\n");
						system("pause");
						system("cls");
						printf("Player 1's board:\n");
						print_board(p1);
						printf("Player 2's board:\n");
						print_board(p2_display);
					}
				}
				else {//Makes the player select coordinates again if they're not between 0 and 9
					printf("Invalid input, your coordinates must be between 0-9.\n");
					system("pause");
					system("cls");
					printf("Player 1's board:\n");
					print_board(p1);
					printf("Player 2's board:\n");
					print_board(p2_display);
				}
			}
				system("pause");
				system("cls");
				turn++;
		}
	}
	printf("Player 1's board:\n");
	print_board(p1);
	printf("Player 2's board:\n");
	print_board(p2_display);
	p1_stats.ratio = ((double)p1_stats.hits / (double)p1_stats.shots) * 100;//Sets the ratio to whatever it should be for both players
	p2_stats.ratio = ((double)p2_stats.hits / (double)p2_stats.shots) * 100;
	if (p1_sunk == 5) {//Determines which player won
		printf("**** Player 1 wins! ****\n");
		fprintf(outfile, "Player 1 wins, Player 2 loses\n\n");
	}
	else {
		printf("**** Player 2 wins! ****\n");
		fprintf(outfile, "Player 2 wins, Player 1 loses\n\n");
	}
	print_stats(p1_stats, outfile, 1);
	fprintf(outfile, "\n");
	print_stats(p2_stats, outfile, 2);//Stats are printed to outfile
	printf("Statistics output to logfile successfully!\n");
	system("pause");
	system("cls");
	fclose(outfile);//File is closed
	welcome_screen();//Player is returned to menu
}

/*
Function name: init_board
Description: This function initializes a board to "~" for all spaces
Input parameters: A board (2d array)
Returns: Doesn't return anything, but sets the board to "~"
Preconditions: play_game has started
Postconditions: all spaces within the BORDER of the board have been set to "~"
*/
void init_board(char board[][BORDER]) {
	for (int i = 0; i < BORDER; i++) {
		for (int j = 0; j < BORDER; j++) {
			board[i][j] = '~';
		}
		printf("\n");
	}
}

/*
Function name: print_board
Description: This function displays a board, and the coresponding coordinates above and beside the board to make it easier to read
Input parameters: A board (2d array)
Returns: None
Preconditions: The board has been initialized
Postconditions: The board has been printed
*/
void print_board(char board[][BORDER]) {
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	for (int i = 0; i < BORDER; i++) {
		printf("%d ", i);
		for (int j = 0; j < BORDER; j++) {
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

/*
Function name: manual_board
Description: This function runs a loop to call place_ship, in order to have the player manually place their ships on their board
Input parameters: A board (2d array)
Returns: Nothing, but it changes the coordinates where the ships are on the board
Preconditions: The board has been initialized
Postconditions: All 5 ships have successfully been placed on the board
*/
void manual_board(char board[][BORDER]) {
	for (int i = 1; i < 6; i++) {
		place_ship(board, i);
	}
}

/*
Function name: manual_board
Description: This function randomly places 5 ships on a board, checking that they are within the borders and don't overlap
Input parameters: A board (2d array)
Returns: Nothing, but it changes the coordinates where the ships are on the board
Preconditions: The board has been initialized
Postconditions: All 5 ships have successfully been placed on the board
*/
void random_board(char board[][BORDER]) {
	int x_coord = 0, y_coord = 0, dir = 0, valid = 0;
	for (int i = 1; i < 6; i++) {
		switch (i) {
		case 1:
			x_coord = rand() % 10;
			y_coord = rand() % 10;
			board[y_coord][x_coord] = 'c';
			for (int n = 0; n == 0;) {
				dir = rand() % 4 + 1;
				valid = check_direction(board, i, x_coord, y_coord, dir);
				if (valid == 1)
					n = 1;
			}
			break;
		case 2:
			for (int n = 0; n == 0;) {
				x_coord = rand() % 10;
				y_coord = rand() % 10;
				if (board[y_coord][x_coord] == '~') {
					n = 1;
					board[y_coord][x_coord] = 'b';
				}
			}
			for (int n = 0; n == 0;) {
				dir = rand() % 4 + 1;
				valid = check_direction(board, i, x_coord, y_coord, dir);
				if (valid == 1)
					n = 1;
			}
			break;
		case 3:
			for (int n = 0; n == 0;) {
				x_coord = rand() % 10;
				y_coord = rand() % 10;
				if (board[y_coord][x_coord] == '~') {
					n = 1;
					board[y_coord][x_coord] = 'r';
				}
			}
			for (int n = 0; n == 0;) {
				dir = rand() % 4 + 1;
				valid = check_direction(board, i, x_coord, y_coord, dir);
				if (valid == 1)
					n = 1;
			}
			break;
		case 4:
			for (int n = 0; n == 0;) {
				x_coord = rand() % 10;
				y_coord = rand() % 10;
				if (board[y_coord][x_coord] == '~') {
					n = 1;
					board[y_coord][x_coord] = 's';
				}
			}
			for (int n = 0; n == 0;) {
				dir = rand() % 4 + 1;
				valid = check_direction(board, i, x_coord, y_coord, dir);
				if (valid == 1)
					n = 1;
			}
			break;
		case 5:
			for (int n = 0; n == 0;) {
				x_coord = rand() % 10;
				y_coord = rand() % 10;
				if (board[y_coord][x_coord] == '~') {
					n = 1;
					board[y_coord][x_coord] = 'd';
				}
			}
			for (int n = 0; n == 0;) {
				dir = rand() % 4 + 1;
				valid = check_direction(board, i, x_coord, y_coord, dir);
				if (valid == 1)
					n = 1;
			}
			break;
		}
	}
}

/*
Function name: place_ship
Description: This function has the user manually place 5 ships on their board, checking that the ships don't overlap or go outside the borders
Input parameters: A board (2d array), the ship's corresponding number (an int)
Returns: Nothing, but it initializes the points where a ship will begin
Preconditions: The board has been initialized
Postconditions: All 5 ships have successfully been placed on the board
*/
void place_ship(char board[][BORDER], int ship_num) {
	int x_coord = 0, y_coord = 0, dir = 0, valid = 0;
	switch (ship_num) {
	case 1:
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Choose the starting coordinates for your Carrier (X Y)\nNote: 0 0 is the upper left corner and 9 9 is the bottom right corner: ");
			scanf("%d %d", &x_coord, &y_coord);
			if (x_coord > 9 || x_coord < 0 || y_coord > 9 || y_coord < 0) {
				printf("Invalid input, please enter coordinates within the range of the board.\n");
				system("pause");
				system("cls");
			}
			else {
				i = 1;
				board[y_coord][x_coord] = 'c';
			}
		}
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Which direction from your coordniate would you like to place your Carrier? \n(must have 4 free spaces)\n1. Left\n2. Right\n3. Up\n4. Down\n");
			scanf("%d", &dir);
			if (dir == 1 || dir == 2 || dir == 3 || dir == 4) {
				i = 1;
				valid = check_direction(board, ship_num, x_coord, y_coord, dir);
				if (valid == 0) {
					i = 0;
					printf("Invalid input, make sure your ship is within the boarders.\n");
					system("pause");
					system("cls");
				}
			}
			else {
				printf("Invalid input, please try again.\n");
				system("pause");
				system("cls");
			}
		}
		system("cls");
		break;
	case 2:
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Choose the starting coordinates for your Battleship (X Y)\nNote: 0 0 is the upper left corner and 9 9 is the bottom right corner: ");
			scanf("%d %d", &x_coord, &y_coord);
			if (x_coord > 9 || x_coord < 0 || y_coord > 9 || y_coord < 0) {
				printf("Invalid input, please enter coordinates within the range of the board.\n");
				system("pause");
				system("cls");
			}
			else {
				valid = check_overlap(board, ship_num, x_coord, y_coord, 0);
				if (valid == 1) {
					i = 1;
					board[y_coord][x_coord] = 'b';
				}
				else {
					printf("Invalid input, this coordinate overlaps with another ship.\n");
					system("pause");
					system("cls");
				}
			}
		}
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Which direction from your coordniate would you like to place your Battleship? \n(must have 3 free spaces)\n1. Left\n2. Right\n3. Up\n4. Down\n");
			scanf("%d", &dir);
			if (dir == 1 || dir == 2 || dir == 3 || dir == 4) {
				i = 1;
				valid = check_direction(board, ship_num, x_coord, y_coord, dir);
				if (valid == 0) {
					i = 0;
					printf("Invalid input, make sure your ship is within the boarders and doesn't overlap with another ship.\n");
					system("pause");
					system("cls");
				}
			}
			else {
				printf("Invalid input, please try again.\n");
				system("pause");
				system("cls");
			}
		}
		system("cls");
		break;
	case 3:
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Choose the starting coordinates for your Cruiser (X Y)\nNote: 0 0 is the upper left corner and 9 9 is the bottom right corner: ");
			scanf("%d %d", &x_coord, &y_coord);
			if (x_coord > 9 || x_coord < 0 || y_coord > 9 || y_coord < 0) {
				printf("Invalid input, please enter coordinates within the range of the board.\n");
				system("pause");
				system("cls");
			}
			else {
				valid = check_overlap(board, ship_num, x_coord, y_coord, 0);
				if (valid == 1) {
					i = 1;
					board[y_coord][x_coord] = 'r';
				}
				else {
					printf("Invalid input, this coordinate overlaps with another ship.\n");
					system("pause");
					system("cls");
				}
			}
		}
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Which direction from your coordniate would you like to place your Cruiser? \n(must have 2 free spaces)\n1. Left\n2. Right\n3. Up\n4. Down\n");
			scanf("%d", &dir);
			if (dir == 1 || dir == 2 || dir == 3 || dir == 4) {
				i = 1;
				valid = check_direction(board, ship_num, x_coord, y_coord, dir);
				if (valid == 0) {
					i = 0;
					printf("Invalid input, make sure your ship is within the boarders and doesn't overlap with another ship.\n");
					system("pause");
					system("cls");
				}
			}
			else {
				printf("Invalid input, please try again.\n");
				system("pause");
				system("cls");
			}
		}
		system("cls");
		break;
	case 4:
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Choose the starting coordinates for your Submarine (X Y)\nNote: 0 0 is the upper left corner and 9 9 is the bottom right corner: ");
			scanf("%d %d", &x_coord, &y_coord);
			if (x_coord > 9 || x_coord < 0 || y_coord > 9 || y_coord < 0) {
				printf("Invalid input, please enter coordinates within the range of the board.\n");
				system("pause");
				system("cls");
			}
			else {
				valid = check_overlap(board, ship_num, x_coord, y_coord, 0);
				if (valid == 1) {
					i = 1;
					board[y_coord][x_coord] = 's';
				}
				else {
					printf("Invalid input, this coordinate overlaps with another ship.\n");
					system("pause");
					system("cls");
				}
			}
		}
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Which direction from your coordniate would you like to place your Submarine? \n(must have 2 free spaces)\n1. Left\n2. Right\n3. Up\n4. Down\n");
			scanf("%d", &dir);
			if (dir == 1 || dir == 2 || dir == 3 || dir == 4) {
				i = 1;
				valid = check_direction(board, ship_num, x_coord, y_coord, dir);
				if (valid == 0) {
					i = 0;
					printf("Invalid input, make sure your ship is within the boarders and doesn't overlap with another ship.\n");
					system("pause");
					system("cls");
				}
			}
			else {
				printf("Invalid input, please try again.\n");
				system("pause");
				system("cls");
			}
		}
		system("cls");
		break;
	case 5:
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Choose the starting coordinates for your Destroyer (X Y)\nNote: 0 0 is the upper left corner and 9 9 is the bottom right corner: ");
			scanf("%d %d", &x_coord, &y_coord);
			if (x_coord > 9 || x_coord < 0 || y_coord > 9 || y_coord < 0) {
				printf("Invalid input, please enter coordinates within the range of the board.\n");
				system("pause");
				system("cls");
			}
			else {
				valid = check_overlap(board, ship_num, x_coord, y_coord, 0);
				if (valid == 1) {
					i = 1;
					board[y_coord][x_coord] = 'd';
				}
				else {
					printf("Invalid input, this coordinate overlaps with another ship.\n");
					system("pause");
					system("cls");
				}
			}
		}
		for (int i = 0; i == 0;) {
			print_board(board);
			printf("Which direction from your coordniate would you like to place your Destroyer? \n(must have 1 free space)\n1. Left\n2. Right\n3. Up\n4. Down\n");
			scanf("%d", &dir);
			if (dir == 1 || dir == 2 || dir == 3 || dir == 4) {
				i = 1;
				valid = check_direction(board, ship_num, x_coord, y_coord, dir);
				if (valid == 0) {
					i = 0;
					printf("Invalid input, make sure your ship is within the boarders and doesn't overlap with another ship.\n");
					system("pause");
					system("cls");
				}
			}
			else {
				printf("Invalid input, please try again.\n");
				system("pause");
				system("cls");
			}
		}
		break;
	}
}

/*
Function name: check_direction
Description: This function checks if the direction the user selected for their ship is within the border of the board and doesn't overlap with any ships
Input parameters: A board (2d array), a ship's corresponding number, the selected coordinates, the selected direction
Returns: Either 0 or 1. If the program returns 0, the ship wasn't able to be placed, if it returned 1, the ship was successfully placed
Preconditions: The board has been initialized and the player has selected their coordinates and direction
Postconditions: The ship has either been placed or wasn't placed due to the border or overlap
*/
int check_direction(char board[][BORDER], int ship_num, int x_coord, int y_coord, int dir) {
	if (ship_num == 1) {//This function has many if statements, to check each direction of a ship
		if (dir == 1){
			if (x_coord - 4 < 0)
				return 0;
			else {
				board[y_coord][x_coord - 1] = 'c';
				board[y_coord][x_coord - 2] = 'c';
				board[y_coord][x_coord - 3] = 'c';
				board[y_coord][x_coord - 4] = 'c';
				return 1;
			}
		}
		else if (dir == 2) {
			if (x_coord + 4 > 9)
				return 0;
			else {
				board[y_coord][x_coord + 1] = 'c';
				board[y_coord][x_coord + 2] = 'c';
				board[y_coord][x_coord + 3] = 'c';
				board[y_coord][x_coord + 4] = 'c';
				return 1;
			}
		}
		else if (dir == 3) {
			if (y_coord - 4 < 0)
				return 0;
			else {
				board[y_coord-1][x_coord] = 'c';
				board[y_coord-2][x_coord] = 'c';
				board[y_coord-3][x_coord] = 'c';
				board[y_coord-4][x_coord] = 'c';
				return 1;
			}
		}
		else if (dir == 4) {
			if (y_coord + 4 > 9)
				return 0;
			else {
				board[y_coord + 1][x_coord] = 'c';
				board[y_coord + 2][x_coord] = 'c';
				board[y_coord + 3][x_coord] = 'c';
				board[y_coord + 4][x_coord] = 'c';
				return 1;
			}
		}
	}
	else if (ship_num == 2) {
		if (dir == 1) {
			if (x_coord - 3 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord - 1] = 'b';
					board[y_coord][x_coord - 2] = 'b';
					board[y_coord][x_coord - 3] = 'b';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 2) {
			if (x_coord + 3 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord + 1] = 'b';
					board[y_coord][x_coord + 2] = 'b';
					board[y_coord][x_coord + 3] = 'b';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 3) {
			if (y_coord - 3 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord-1][x_coord] = 'b';
					board[y_coord-2][x_coord] = 'b';
					board[y_coord-3][x_coord] = 'b';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 4) {
			if (y_coord + 3 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord + 1][x_coord] = 'b';
					board[y_coord + 2][x_coord] = 'b';
					board[y_coord + 3][x_coord] = 'b';
					return 1;
				}
				else
					return 0;
			}
		}
	}
	else if (ship_num == 3) {
		if (dir == 1) {
			if (x_coord - 2 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord - 1] = 'r';
					board[y_coord][x_coord - 2] = 'r';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 2) {
			if (x_coord + 2 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord + 1] = 'r';
					board[y_coord][x_coord + 2] = 'r';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 3) {
			if (y_coord - 2 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord - 1][x_coord] = 'r';
					board[y_coord - 2][x_coord] = 'r';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 4) {
			if (y_coord + 2 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord + 1][x_coord] = 'r';
					board[y_coord + 2][x_coord] = 'r';
					return 1;
				}
				else
					return 0;
			}
		}
	}
	else if (ship_num == 4) {
		if (dir == 1) {
			if (x_coord - 2 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord - 1] = 's';
					board[y_coord][x_coord - 2] = 's';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 2) {
			if (x_coord + 2 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord + 1] = 's';
					board[y_coord][x_coord + 2] = 's';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 3) {
			if (y_coord - 2 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord - 1][x_coord] = 's';
					board[y_coord - 2][x_coord] = 's';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 4) {
			if (y_coord + 2 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord + 1][x_coord] = 's';
					board[y_coord + 2][x_coord] = 's';
					return 1;
				}
				else
					return 0;
			}
		}
	}
	else if (ship_num == 5) {
		if (dir == 1) {
			if (x_coord - 1 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord - 1] = 'd';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 2) {
			if (x_coord + 1 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord][x_coord + 1] = 'd';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 3) {
			if (y_coord - 1 < 0)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord - 1][x_coord] = 'd';
					return 1;
				}
				else
					return 0;
			}
		}
		else if (dir == 4) {
			if (y_coord + 1 > 9)
				return 0;
			else {
				if (check_overlap(board, ship_num, x_coord, y_coord, dir) == 1) {
					board[y_coord + 1][x_coord] = 'd';
					return 1;
				}
				else
					return 0;
			}
		}
	}
}

/*
Function name: check_overlap
Description: This function checks that the coordinate where the ship is being placed is a "water" space, or if the direction has been choosen,
			 that the ship doesn't overlap with another ship
Input parameters: A board (2d array), a ship's corresponding number, the selected coordinates, the selected direction
Returns: Either 0 or 1. 1 if the ship can be placed without any overlap and 0 if there is overlap
Preconditions: The board has been initialized and the player has selected their coordinates, as well as a direction (if they haven't 0 is used in place of direction)
Postconditions: It has been determined that the ship will or won't overlap with another ship
*/
int check_overlap(char board[][BORDER], int ship_num, int x_coord, int y_coord, int dir) {
	if (dir == 0) {
		if (board[y_coord][x_coord] != '~')
			return 0;
		else
			return 1;
	}
	else {
		if (ship_num == 2) { //many if statements, to check the amount of spaces that needs to be checked for each ship
			if (dir == 1) {
				if (board[y_coord][x_coord - 1] == '~' && board[y_coord][x_coord - 2] == '~' && board[y_coord][x_coord - 3] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 2) {
				if (board[y_coord][x_coord + 1] == '~' && board[y_coord][x_coord + 2] == '~' && board[y_coord][x_coord + 3] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 3) {
				if (board[y_coord-1][x_coord] == '~' && board[y_coord-2][x_coord] == '~' && board[y_coord-3][x_coord] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 4) {
				if (board[y_coord + 1][x_coord] == '~' && board[y_coord + 2][x_coord] == '~' && board[y_coord + 3][x_coord] == '~')
					return 1;
				else
					return 0;
			}
		}
		if (ship_num == 3 || ship_num == 4) {
			if (dir == 1) {
				if (board[y_coord][x_coord - 1] == '~' && board[y_coord][x_coord - 2] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 2) {
				if (board[y_coord][x_coord + 1] == '~' && board[y_coord][x_coord + 2] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 3) {
				if (board[y_coord - 1][x_coord] == '~' && board[y_coord - 2][x_coord] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 4) {
				if (board[y_coord + 1][x_coord] == '~' && board[y_coord + 2][x_coord] == '~')
					return 1;
				else
					return 0;
			}
		}
		if (ship_num == 5) {
			if (dir == 1) {
				if (board[y_coord][x_coord - 1] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 2) {
				if (board[y_coord][x_coord + 1] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 3) {
				if (board[y_coord - 1][x_coord] == '~')
					return 1;
				else
					return 0;
			}
			else if (dir == 4) {
				if (board[y_coord + 1][x_coord] == '~')
					return 1;
				else
					return 0;
			}
		}
	}
}

/*
Function name: first_turn
Description: This function simply chooses 0 or 1, and using that number, the player who goes first is determined
Input parameters: None
Returns: None
Preconditions: Both the player's and computer's board have been set and the game is ready to begin
Postconditions: 0 or 1 is selected
*/
int first_turn(void) {
	return rand() % 2;
}

/*
Function name: hit_or_miss
Description: This function simply checks if the coordinates selected is a hit or a miss "~" or anything else, since I already check that the player isn't
			 firing at a "m" or "*"
Input parameters: A board (2d array), and the selected coordinates
Returns: Either 0 or 1. 1 if it's a hit and 0 if it's a miss
Preconditions: The player has selected valid coordinates to fire at, and the coordinates are not "m" or "*"
Postconditions: It has been determined that the shot was a hit or miss
*/
int hit_or_miss(char board[][BORDER], int x_coord, int y_coord) {
	if (board[y_coord][x_coord] == '~')
		return 0;
	else
		return 1;
}

/*
Function name: ship_sunk
Description: This function checks that the coordinate where the ship is being placed is a "water" space, or if the direction has been choosen,
that the ship doesn't overlap with another ship
Input parameters: A board (2d array), the selected coordinates, the number of ships a player has sunk, the log file
Returns: The current number of ships a player has sunk, also outputs to the log file if the player sunk a ship, and if so, what ship
Preconditions: The player has fired, and hit a ship and the log file is open and able to be writen to
Postconditions: It has been determined if the player did or didn't sink a ship
*/
int ship_sunk(char board[][BORDER], int x_coord, int y_coord, int sunk, FILE *outfile) {
	char hit_ship = board[y_coord][x_coord];
	board[y_coord][x_coord] = '*';
	for (int i = 0; i < BORDER; i++) {
		for (int j = 0; j < BORDER; j++) {
			if (board[i][j] == hit_ship)
				return sunk;
		}
	}
	if (hit_ship == 'c') {
		printf("The Carrier was sunk!\n");
		fprintf(outfile, " Sunk Carrier!\n");
		return sunk + 1;
	}
	else if (hit_ship == 'b') {
		printf("The Battleship was sunk!\n");
		fprintf(outfile, " Sunk Battleship!\n");
		return sunk + 1;
	}
	else if (hit_ship == 'r') {
		fprintf(outfile, " Sunk Cruiser!\n");
		printf("The Cruiser was sunk!\n");
		return sunk + 1;
	}
	else if (hit_ship == 's') {
		fprintf(outfile, " Sunk Submarine!\n");
		printf("The Submarine was sunk!\n");
		return sunk + 1;
	}
	else if (hit_ship == 'd') {
		fprintf(outfile, " Sunk Destroyer!\n");
		printf("The Destroyer was sunk!\n");
		return sunk + 1;
	}
}

/*
Function name: print_stats
Description: This function just prints a player's stats to the log file
Input parameters: A Stats of a player's stats, the log file, the player number
Returns: Nothing, but prints the player's stats to the log file
Preconditions: The game has been played and finished, and the stats for each player has been updated throughout the game, and the log file is open
Postconditions: The player's stats have successfully been written to the log file
*/
void print_stats(Stats p, FILE *outfile, int p_num) {
	fprintf(outfile, "**** Player %d Stats ****\n", p_num);
	fprintf(outfile, "Number Hits: %d\nNumber Misses: %d\nTotal Shots: %d\nHit/Miss Ratio: %.2lf%%\n", p.hits, p.misses, p.shots, p.ratio);
}