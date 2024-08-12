#include <stdio.h>
#include "types.h"

void goYellow(Player* player)
{

}

void go(Player* player)
{
	switch (player->color)
	{
	case YELLOW: goYellow(player);
		break;
	case BLUE: goYellow(player);
		break;
	case RED: goYellow(player);
		break;
	case GREEN: goYellow(player);
	}
}