#include <stdio.h>
#include "types.h"
#include "functions.h"
#include "logic.h"




// game behaviour for the yellow player
void goYellow(Player* player)
{




}

// game behaviour for the blue player
void goBlue(Player* player)
{




}

// game behaviour for the red player
void goRed(Player* player)
{




}

// game behaviour for the green player
void goGreen(Player* player)
{




}



void go(Player* player)
{
	switch (player->color)
	{
	case YELLOW:goYellow(player);
				break;
	case BLUE:	goBlue(player);
				break;
	case RED:	goRed(player);
				break;
	case GREEN:	goGreen(player);
	}
}