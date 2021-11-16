/*
Name: Anthony George
Program: PA 6 - Battleship
Description: This program has the user play the game "Battleship" against the computer, and prints out the game's statistics to a log file
1. The main function opens a menu where the player decides if they want to see the rules, play the game or exit the program.
2. When the player starts the game, 3 "boards" are initialized to water ("~") the boards are p1's, p2's and the display of p2's.
3. After initializing the boards, the program asks the player if they'd like to place their ships manually or randomly.
4. If the player selects randomly, the program generates random numbers and directions to place the ships, checking that there is no overlap
   and that all ships are within the borders of the board.
5. If the player selects manually, the program asks for them to enter coordinates and directions for each ship. Then checks that the ship
   is within the borders of the board and doesn't overlap with any other ships.
   (Note: I noticed that for steps 4 and 5, the program could get stuck in an infinte loop. This would occur if 3 ships and a boarder, or 4 ships
    create a circle with only 1 or 2 water spaces in the middle, this would mean a ship could initialized there, but couldn't actually be placed
	since the direction check will say every direction is invalid. Knowing that this would only happen with the last 2 ships (Destroyer or Submarine)
	if I wanted to go back and fix it, I could make the program check that for the Submarine, there is 2 spaces of water next to each other next to the
	coordinate, and 1 space next to the coordinate for the Destroyer).
6. Once the player's board is set, the computer's board is randomly generated.
7. The game then randomly decides who gets to go first, and the game begins
8. On each turn, a coordinate is selected (randomly for computer) manually for player, and the program says if it was a hit or miss, and if a ship was sunk.
   (Note: Technically, if the computer gets a hit, it should check the coordinates around that coordinate, but I wasn't too sure how to code that)
9. The boards are updated and displayed (p1's board and p2's board without the ships) 
10. The players take turns firing at the opponent's board until all 5 ships on a board are sunk (steps 7-8 are repeated)
11. After all ships are sunk, the program then determines the winner and outputs the stats of the game to the log file.
12. The log file is closed and the program returns to the menu (step 1)
*/
#include "functions.h"

//main simply starts the menu
int main(void) {
	welcome_screen();
	return 0;
}