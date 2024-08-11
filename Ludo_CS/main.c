#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "logic.h"

#define PutI(i) printf("%d\n",i)

int main()
{
	//FILE* fp;

	//freopen_s(&fp, "output.txt", "w", stdout);


	printf("\t\t\t\t\t___ LUDO CS ___ \n\t\t\t\t      by Bhasura Jayaweera\n\n");
	for (short i = 0; i < 100; i++) printf("_");
	NEWLINE;
	for (short i = 0; i < 100; i++) printf("#");
	NEWLINE; NEWLINE;

	Player player1 = initPlayer(YELLOW);
	Player player2 = initPlayer(BLUE);
	Player player3 = initPlayer(RED);
	Player player4 = initPlayer(GREEN);

	printf("The yellow player has four (04) pieces named Y1, Y2, Y3, and Y4.\n");
	printf("The blue player has four (04) pieces named B1, B2, B3, and B4.\n");
	printf("The red player has four (04) pieces named R1, R2, R3, and R4.\n");
	printf("The green player has four (04) pieces named G1, G2, G3, and G4.\n");

	NEWLINE;

	start(&player1, &player2, &player3, &player4);

	while (1)
	{
		round(&player1);
	}

}