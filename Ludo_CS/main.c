#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "logic.h"
#include "functions.h"


int main()
{
	printf("\t\t\t\t\t___ LUDO CS ___ \n\t\t\t\t      by Bhasura Jayaweera\n\n");
	LINE;	HLINE;	NEWLINE;

	// making the player instants
	Player player1 = initPlayer(YELLOW);
	Player player2 = initPlayer(BLUE);
	Player player3 = initPlayer(RED);
	Player player4 = initPlayer(GREEN);

	NEWLINE;

	// starts the game by passing the player addresses
	start(&player1, &player2, &player3, &player4);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// for testing purposes
	//
	//Player_p mP = NULL;
	//Player_p cP = NULL;
	//Block_p mp = NULL;
	//Block_p cp = NULL;
	//
	//for (short i = 0; i < 4; i++)
	//{
	//	if (players[i]->color == YELLOW)
	//	{
	//
	//	}
	//
	//	if (players[i]->color == RED)
	//	{
	//
	//	}
	//}
	//
	//moveBlock(mP, mp, 6);
	//
	//stats();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	LINE;

	// game loop
	while (!over)
	{
		game_round_runner(&player1, &player2, &player3, &player4);
		LINE;
	}

	NEWLINE; HLINE; NEWLINE;
	printf("%s Player won the game !!!\n\n", WPlayer->name);
}