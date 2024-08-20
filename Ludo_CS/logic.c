#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"
#include "types.h"
#include "functions.h"
#include "move.h"


// globals
int game_round_count = 0;
Piece_p v_map_p[4][4] = { 0 };	// virtual map for pieces
Block_p v_map_b[4][2] = { 0 };	// virtual map for blocks


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main functions
// These fucntions is called in the main method


// initialize players
Player initPlayer(Color color)
{
	Player player;
	player.b[0] = NULL;
	player.b[1] = NULL;
	player.color = color;
	player.order = NONE;
	player.current_roll = NONE;
	player.six_count = NONE;
	player.name = (char*)getName(color);

	for (short i = 0; i < 4; i++)
	{
		player.p[i].move = 1.0f;
		player.p[i].color = color;
		player.p[i].block = false;
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].distance = 0;
		player.p[i].direction = NONE;
		player.p[i].capture_count = 0;
		player.p[i].mystery_count = 0;
	}

	printf("The %s player has four (04) pieces named %s, %s, %s, and %s.\n", player.name, getPID(&player.p[0]), getPID(&player.p[1]), getPID(&player.p[2]), getPID(&player.p[3]) );

	return player;
}


// Start the game
void start(Player_p player1, Player_p player2, Player_p player3, Player_p player4)
{
	// seeding the time for the random number generator
	srand((unsigned int)time(NULL));

	short max_player;

	do
	{
		printf("Yellow rolls %d\n", rolls(player1, false));
		printf("Blue rolls %d\n", rolls(player2, false));
		printf("Red rolls %d\n", rolls(player3, false));
		printf("Green rolls %d\n", rolls(player4, false));

		max_player = getMax(player1, player2, player3, player4);

	} while (max_player < 0);

	setOrder(player1, player2, player3, player4, max_player);

	// maping the virtual map for pieces
	
	v_map_p[0][0] = &(player1->p[0]);
	v_map_p[0][1] = &(player1->p[1]);
	v_map_p[0][2] = &(player1->p[2]);
	v_map_p[0][3] = &(player1->p[3]);
		 
	v_map_p[1][0] = &(player2->p[0]);
	v_map_p[1][1] = &(player2->p[1]);
	v_map_p[1][2] = &(player2->p[2]);
	v_map_p[1][3] = &(player2->p[3]);
		 
	v_map_p[2][0] = &(player3->p[0]);
	v_map_p[2][1] = &(player3->p[1]);
	v_map_p[2][2] = &(player3->p[2]);
	v_map_p[2][3] = &(player3->p[3]);
		 
	v_map_p[3][0] = &(player4->p[0]);
	v_map_p[3][1] = &(player4->p[1]);
	v_map_p[3][2] = &(player4->p[2]);
	v_map_p[3][3] = &(player4->p[3]);

}


// one game round of a game
void game_round_runner(Player_p player1, Player_p player2, Player_p player3, Player_p player4)
{
	game_round_count++;
	printf(">>> Game round - %d\n", game_round_count);

	// Calling for the move fucntion for each player.
	while (move(player1));
	while (move(player2));
	while (move(player3));
	while (move(player4));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////