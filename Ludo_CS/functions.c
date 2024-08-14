#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functions.h"
#include "types.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helper functions


// rolls the die
short rollDice()
{
	return (short)rand() % 6 + 1;
}


// rolls the dice for players
short rolls(Player* player)
{
	player->current_roll = rollDice();

	return player->current_roll;
}


// get the maximum roll number from players
short getMax(Player* p1, Player* p2, Player* p3, Player* p4)
{
	short a = p1->current_roll;
	short b = p2->current_roll;
	short c = p3->current_roll;
	short d = p4->current_roll;

	if (a > b && a > c && a > d)
		return p1->color;
	else if (b > a && b > c && b > d)
		return p2->color;
	else if (c > a && c > b && c > d)
		return p3->color;
	else if (d > a && d > b && d > c)
		return p4->color;
	else
		return -1;
}


// swap the order of players
void setOrder(Player* p1, Player* p2, Player* p3, Player* p4, Color max_player)
{
	Player temp1 = *p1;
	Player temp2 = *p2;
	Player temp3 = *p3;

	switch (max_player)
	{
	case BLUE:
		*p1 = *p2;
		*p2 = *p3;
		*p3 = *p4;
		*p4 = temp1;
		break;

	case RED:
		*p1 = *p3;
		*p2 = *p4;
		*p3 = temp1;
		*p4 = temp2;
		break;

	case GREEN:
		*p1 = *p4;
		*p2 = temp1;
		*p3 = temp2;
		*p4 = temp3;
	}

	p1->order = 0;
	p2->order = 1;
	p3->order = 2;
	p4->order = 3;

}


// translate integer location to a cell id (0 - 51)
short getLoc(short position)
{
	if (position < 0)
		return ((52 + position) % 52);
	else
		return (position % 52);
}


// get the destination cell for a dice roll
short getPosition(Player* player, Piece* piece)
{
	return getLoc(piece->location + (player->current_roll * piece->direction));
}


// get the number of pieces in the board of a given player
short getBoardPieceCount(Player* player)
{
	short count = 0;
	for (short i = 0; i < 4; i++)
	{
		if (player->p[i].location != BASE)
			count++;
	}
	return count;
}


// choose and return one piece form the pieces in the base
Piece* getBasePiece(Player* player)
{
	for (short i = 0; i < 4; i++)
	{
		if (player->p[i].location = BASE)
			return &(player->p[i]);
	}
	return NULL;
}


// checking whether the cell is available for a piece of a specific player
bool isCellClear(Player* player, short location)
{
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			if (location == (v_map[i][j])->location && player->color != (v_map[i][j])->color && (v_map[i][j])->block)
				return false;
		}
	}
	return true;
}


// get the color of a player from a location
Color getColor(short location)
{
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			if (location == (v_map[i][j])->location)
				return (v_map[i][j])->color;
		}
	}
	return NONE;
}


// get the piece id of a specific location
short getPieceID(short location)
{
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			if (location == (v_map[i][j])->location)
				return (v_map[i][j])->id;
		}
	}
	return NONE;
}


// get the name of a player from color
const char* getName(Color color)
{
	switch (color)
	{
	case YELLOW: return "Yellow";
	case BLUE: return "Blue";
	case RED: return "Red";
	case GREEN: return "Green";
	}
}


// return the diatance to the home of a piece
short getHomeDistance(Piece* piece)
{
	if (piece->direction == CLOCKWISE)
		return (short)(56 - piece->location + (short)piece->color);	// simplified 52 - (location) + (approach_cell) + 6
	else
		return (short)(piece->location - (short)piece->color + 8);	// simplified (location) - (approach_cell) + 6
}


// check the range and returns the destination for the piece to move 
short isRangeClear(Player* player, Piece* piece)
{
	if ((v_map[0][0])->color == piece->color)
		return CANBLOCK;

	if (piece->direction == CLOCKWISE)
	{
		for (short i = piece->location; i < (piece->location + player->current_roll); i++)
		{
			for (short i = 0; i < 4; i++)
			{
				for (short j = 0; j < 4; j++)
				{
					if ((v_map[0][0])->block && (v_map[i][j])->color != piece->color)
						return (v_map[i][j])->location - 1;
				}
			}
		}

	}
	else    // if the direction is counterclockwise
	{
		for (short i = piece->location; i < (piece->location - player->current_roll); i--)
		{
			for (short i = 0; i < 4; i++)
			{
				for (short j = 0; j < 4; j++)
				{
					if ((v_map[i][j])->color == piece->color)
						return CANBLOCK;
					else if ((v_map[0][0])->block && (v_map[i][j])->color != piece->color)
						return (v_map[i][j])->location - 1;
				}
			}
		}
	}
	{

	}
	{

	}
}

