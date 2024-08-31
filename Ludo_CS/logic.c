#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"
#include "types.h"
#include "functions.h"
#include "players.h"


// globals
int game_round_count = 0;
char map[52] = { 0 };			// map for the board path
Player_p players[4] = { 0 };	// pointers for players
short mystery_cell =  -52;		// mystery cell (0-52)
short traffic_count = 0;		// count the traffic in order to stop the game in case of an unavoidable path block
bool over;
Player_p WPlayer = NULL;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].distance = 0;
		player.p[i].direction = NONE;
		player.p[i].capture_count = 0;
		player.p[i].mystery_count = 0;
		player.p[i].block = false;
		player.p[i].effect = NONE;
	}

	printf("\033[0mThe %s\033[0m player has four (04) pieces named %s, %s, %s, and %s.\n", player.name, getPID(&player.p[0]), getPID(&player.p[1]), getPID(&player.p[2]), getPID(&player.p[3]) );

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
		printf("\033[0;93mYellow rolls %d\n", rolls(player1, false));
		printf("\033[0;34mBlue rolls %d\n", rolls(player2, false));
		printf("\033[0;31mRed rolls %d\n", rolls(player3, false));
		printf("\033[0;32mGreen rolls %d\n", rolls(player4, false));

		max_player = getMax(player1, player2, player3, player4);

	} while (max_player < 0);

	setOrder(player1, player2, player3, player4, max_player);

	// maping the pointer list for players
	players[0] = player1;
	players[1] = player2;
	players[2] = player3;
	players[3] = player4;
}



// one game round of a game
void game_round_runner(Player_p player1, Player_p player2, Player_p player3, Player_p player4)
{
	game_round_count++;
	static short Mcount;
	if (game_round_count % 4 == 2)
	{
		short temp = abs(RANDELL);
		if (mystery_cell == temp)
			temp = (temp / 2) + 5;
		mystery_cell = temp;
		Mcount = 4;
		printf("\nA mystery cell has spawned in location %hd and will be at this location for the next four rounds.\n", mystery_cell);
	}

	printf(">>> Game round - %d\n", game_round_count);
	// Calling for the move fucntion for each player.
	while (!over && move(player1));
	while (!over && move(player2));
	while (!over && move(player3));
	while (!over && move(player4));

	stats();
	if (mystery_cell != -52)
	{
		printf("\n\033[0mThe mystery cell is at %hd and will be at that location for the next %hd values.\n", mystery_cell, --Mcount);
	}



	if(traffic_count >= 20)
	{
		over = true;
		Player_p maxPlayer = player1;
		short max = player1->p[0].location + player1->p[1].location+ player1->p[2].location+ player1->p[3].location;

		if (player2->p[0].location + player2->p[1].location+ player2->p[2].location+ player2->p[3].location > max)
		{
			maxPlayer = player2;
			max = player2->p[0].location + player2->p[1].location+ player2->p[2].location+ player2->p[3].location;
		}
		if (player3->p[0].location + player3->p[1].location+ player3->p[2].location+ player3->p[3].location > max)
		{
			maxPlayer = player3;
			max = player3->p[0].location + player3->p[1].location+ player3->p[2].location+ player3->p[3].location;
		}
		if (player4->p[0].location + player4->p[1].location+ player4->p[2].location+ player4->p[3].location > max)
		{
			maxPlayer = player4;
		}

		WPlayer = maxPlayer;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////