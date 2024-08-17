#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"
#include "types.h"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// basic helper functions


// rolls the die
short rollDice()
{
	return (short)(rand() % 6 + 1);
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


// returns the piece name
const char* getPID(c_Piece_p piece)
{
	switch (piece->color)
	{
	case YELLOW:	switch (piece->id)
	{
	case 1: return "Y1";
	case 2: return "Y2";
	case 3: return "Y3";
	case 4: return "Y4";
	}
	case BLUE:		switch (piece->id)
	{
	case 1: return "B1";
	case 2: return "B2";
	case 3: return "B3";
	case 4: return "B4";
	}
	case RED:		switch (piece->id)
	{
	case 1: return "R1";
	case 2: return "R2";
	case 3: return "R3";
	case 4: return "R4";
	}
	case GREEN:		switch (piece->id)
	{
	case 1: return "G1";
	case 2: return "G2";
	case 3: return "G3";
	case 4: return "G4";
	}
	}
}


// translate integer location to a cell id (0 - 51)
short getLoc(short position)
{
	if (position < 0)
		return ((52 + position) % 52);
	else
		return (position % 52);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions that deals with original instances


// rolls the dice for players
short rolls(Player_p player)
{
	player->current_roll = rollDice();
	printf("\n%s player rolled %hd.\n", player->name, player->current_roll);

	return player->current_roll;
}


// rolls the dice for the start
short startRolls(Player_p player)
{
	player->current_roll = rollDice();

	return player->current_roll;
}



// toss the coin for a piece and decide the direction
void toss(Piece_p piece)
{
	if (rollDice() % 2)	// head is 0
	{
		piece->direction = CLOCKWISE;
		printf("\tThe coin toss was head, piece %s will move in clockwise direction.\n", getPID(piece));
	}
	else				// tail is 1
	{
		piece->direction = ANTICLOCKWISE;
		printf("\tThe coin toss was tail, piece %s will move in counterclockwise direction.\n", getPID(piece));
	}
}


// capture a piece
void captureP(Piece_p attacking_piece, Piece_p captured_piece)
{
	attacking_piece->capture_count++;
	attacking_piece->location = captured_piece->location;

	captured_piece->location = BASE;
	captured_piece->direction = NONE;
	captured_piece->capture_count = 0;
	captured_piece->mystery_count = 0;
	captured_piece->move = 1.0f;	
	captured_piece->block = false;	

	printf("\t%s piece %s lands on square L%hd, captures %s piece %s, and returns it to the base.\n", getName(attacking_piece->color), getPID(attacking_piece), attacking_piece->location, getName(captured_piece->color), getPID(captured_piece));
}


// swap the order of players
void setOrder(Player_p p1, Player_p p2, Player_p p3, Player_p p4, Color max_player)
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


// choose and return one piece form the pieces in the base
Piece_p getBasePiece(Player_p player)
{
	for (short i = 0; i < 4; i++)
	{
		if (player->p[i].location == BASE)
			return &(player->p[i]);
	}
	return NULL;
}


// choose and return one block form the pieces in the base
Block_p getOneBlock(c_Player_p player)
{
	if (player->b[0] != NULL)
		return player->b[0];
	else if (player->b[1] != NULL)
		return player->b[1];
	else
		return NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions that analys and give outputs


// return the diatance to the home of a piece
short getHomeDistance(c_Piece_p piece)
{
	if (piece->direction == CLOCKWISE)
		return (short)(56 - piece->location + (short)piece->color);	// simplified 52 - (location) + (approach_cell) + 6
	else
		return (short)(piece->location - (short)piece->color + 8);	// simplified (location) - (approach_cell) + 6
}


// get the destination cell for a dice roll
short getDestination(c_Player_p player, c_Piece_p piece)
{
	return getLoc(piece->location + (player->current_roll * piece->direction));
}


// get the number of pieces in the board of a given player
short getBoardPieceCount(c_Player_p player)
{
	short count = 0;
	for (short i = 0; i < 4; i++)
	{
		if (player->p[i].location != BASE)
			count++;
	}
	return count;
}


// get the maximum roll number from players
short getMax(c_Player_p p1, c_Player_p p2, c_Player_p p3, c_Player_p p4)
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions involving virtual maps


// get the piece pointer of a specific location
Piece_p getPiece(short location)
{
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			if (location == (v_map_p[i][j])->location)
				return v_map_p[i][j];
		}
	}
	return NULL;
}


// get the block pointer of a specific location
Block_p getBlock(short location)
{
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 2; j++)
		{
			if ((v_map_b[i][j]) != NULL && location == (v_map_b[i][j])->location)
				return v_map_b[i][j];
		}
	}
	return NULL;
}


// checking whether the cell clear for a player and return the status of the cell
short cellStatus(c_Player_p player, short location)
{
	// checking for blocks
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 2; j++)
		{
			if (v_map_b[i][j] != NULL && location == v_map_b[i][j]->location && player->color != v_map_b[i][j]->color)
				return BLOCKED;
			else if (v_map_b[i][j] != NULL && location == v_map_b[i][j]->location && player->color == v_map_b[i][j]->color)
				return ADDTOBLOCK;
		}
	}	
	
	// checking for pieces
	for (short i = 0; i < 4; i++)
	{
		for (short j = 0; j < 4; j++)
		{
			if (location == v_map_p[i][j]->location && player->color != v_map_p[i][j]->color && !(v_map_p[i][j]->block))
				return CANCAPTURE;
			else if (location == v_map_p[i][j]->location && player->color == v_map_p[i][j]->color && !(v_map_p[i][j]->block))
				return NEWBLOCK;
		}
	}
	return AVAILABLE;		// worst case, when there is nothing in the given cell/location
}


// check the range and returns the destination for the piece to move 
short isRangeClear(c_Player_p player, c_Piece_p piece)
{
	
}
