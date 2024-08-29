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