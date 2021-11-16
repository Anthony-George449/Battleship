#pragma once 
#include <stdio.h>
#include <stdlib.h>

//The border of the boards (makes it so it goes from 0-9)
#define BORDER 10

//my struct for player_stats, includes hits, misses, shots and the ratio of hits/misses
typedef struct player_stats {
	int hits;
	int misses;
	int shots;
	double ratio;
}Stats;

//functions, more in depth in functions.c
void welcome_screen(void);
void display_rules(void);
void play_game(void);
void init_board(char board[][BORDER]);
void print_board(char board[][BORDER]);
void manual_board(char board[][BORDER]);
void random_board(char board[][BORDER]);
void place_ship(char board[][BORDER], int ship_num);
int check_direction(char board[][BORDER], int ship_num, int x_coord, int y_coord, int dir);
int check_overlap(char board[][BORDER], int ship_num, int x_coord, int y_coord, int dir);
int first_turn(void);
int hit_or_miss(char board[][BORDER], int x_coord, int y_coord);
int ship_sunk(char board[][BORDER], int x_coord, int y_coord, int sunk, FILE *outfile);
void print_stats(Stats p, FILE *outfile, int p_num);
 
