#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "logic.h"
#include "functions.h"

extern char map[52];


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

	LINE;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// for testing purposes
	Player_p array[4] = { &player1, &player2, &player3, &player4 };

	for (short i = 0; i < 4; i++)
	{
		if (array[i]->color == GREEN) 
		{
			array[i]->p[3].location = 28;
			addPieceToMap(&(array[i]->p[3]), 28);
		}
		if (array[i]->color == BLUE) 
		{
			Block_p temp = initBlock(array[i], &(array[i]->p[0]), &(array[i]->p[1]));
			temp->location = 2;
			temp->pieces[0]->location = NOTBASE;
			temp->pieces[1]->location = NOTBASE;
			addBlockToMap(temp, 2);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	LINE;

	// game loop
	int c = 0;
	while (c < 50)
	{
		c++;
		game_round_runner(&player1, &player2, &player3, &player4);
		LINE;
	}
}