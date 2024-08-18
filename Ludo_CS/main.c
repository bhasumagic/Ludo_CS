#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "logic.h"

#define PutI(i) printf("%d\n",i)


int main()
{
	printf("\t\t\t\t\t___ LUDO CS ___ \n\t\t\t\t      by Bhasura Jayaweera\n\n");
	LINE;	HASHLINE;	NEWLINE;

	// making the players
	Player player1 = initPlayer(YELLOW);
	Player player2 = initPlayer(BLUE);
	Player player3 = initPlayer(RED);
	Player player4 = initPlayer(GREEN);

	NEWLINE;

	// starts the game by passing the player addresses
	start(&player1, &player2, &player3, &player4);

	LINE;
	
	// for testing purposes
	Player_p array[4] = { &player1, &player2, &player3, &player4 };

	for (short i = 0; i < 4; i++)
	{
		if (array[i]->color == GREEN) 
		{
			array[i]->p[3].location = 15;
		}
		if (array[i]->color == BLUE) 
		{
			initBlock(array[i], &(array[i]->p[0]), &(array[i]->p[1]))->location = 2;
		}
	}





	// game loop
	int c = 0;
	while (c < 100)
	{
		c++;
		game_round_runner(&player1, &player2, &player3, &player4);
		LINE;
	}
}