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

// returns the block name
const char* getBID(c_Block_p block)
{
	switch (block->color)
	{
	case YELLOW:	switch (block->id)
	{
	case 1: return "Y#1";
	case 2: return "Y#2";
	}
	case BLUE:		switch (block->id)
	{
	case 1: return "B#1";
	case 2: return "B#2";
	}
	case RED:		switch (block->id)
	{
	case 1: return "R#1";
	case 2: return "R#2";
	}
	case GREEN:		switch (block->id)
	{
	case 1: return "G#1";
	case 2: return "G#2";
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
short rolls(Player_p player, bool out)
{
	player->current_roll = rollDice();
	//player->current_roll = 6;
	if (out)
		printf("\n%s player rolled %hd.\n", player->name, player->current_roll);

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
void setOrder(Player_p player1, Player_p player2, Player_p player3, Player_p player4, Color max_player)
{
	Player temp1 = *player1;
	Player temp2 = *player2;
	Player temp3 = *player3;

	switch (max_player)
	{
	case BLUE:
		*player1 = *player2;
		*player2 = *player3;
		*player3 = *player4;
		*player4 = temp1;
		break;

	case RED:
		*player1 = *player3;
		*player2 = *player4;
		*player3 = temp1;
		*player4 = temp2;
		break;

	case GREEN:
		*player1 = *player4;
		*player2 = temp1;
		*player3 = temp2;
		*player4 = temp3;
	}

	player1->order = 0;
	player2->order = 1;
	player3->order = 2;
	player4->order = 3;

	printf("\n%s player has the highest roll and will begin the game.\n", player1->name);
	printf("The order of a single round is %s, %s, %s, and %s.\n", player1->name, player2->name, player3->name, player4->name);

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
// Info functions

// return the diatance to the home of a piece
short getHomeDistance(c_Piece_p piece)
{
	if (piece->direction == CLOCKWISE)
		return (short)(58 - piece->distance);
	else
		return (short)(60 - piece->distance);
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
short getMax(c_Player_p player1, c_Player_p player2, c_Player_p player3, c_Player_p player4)
{
	short a = player1->current_roll;
	short b = player2->current_roll;
	short c = player3->current_roll;
	short d = player4->current_roll;

	if (a > b && a > c && a > d)
		return player1->color;
	else if (b > a && b > c && b > d)
		return player2->color;
	else if (c > a && c > b && c > d)
		return player3->color;
	else if (d > a && d > b && d > c)
		return player4->color;
	else
	{
		printf("\n\tThere is no unique max value. rolling again!\n\n");
		return -1;
	}
}

// returns the block size
short blockSize(c_Block_p block)
{
	short count = 0;
	for (short i = 0; i < 4; i++)
		if (block->pieces[i] != NULL)
			count++;
	return count;
}

// check whether a piece is inside a block
bool checkBlock(c_Block_p block, c_Piece_p piece)
{
	for (short i = 0; i < 4; i++)
		if (block->pieces[i] == piece)
			return true;
	return false;
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// fucntions for blocks

// create a block
Block_p initBlock(Player_p player, Piece_p p1, Piece_p p2)
{
	Block_p block = (Block_p)malloc(sizeof(Block));

	// if there is a allocation error and the duplication error
	if (block == NULL || p1 == p2)
		return NULL;

	block->pieces[0] = p1;
	block->pieces[1] = p2;
	block->pieces[2] = NULL;
	block->pieces[3] = NULL;

	block->move = 0.51f;	// the move is set to 1/2 or 0.5 beacuse when creating a block the moving multiple drops by 1/2
	block->color = player->color;

	if (p1->direction != p2->direction)
		block->direction = (getHomeDistance(p1) > getHomeDistance(p2)) ? p1->direction : p2->direction;
	else
		block->direction = p1->direction;

	// updating the block details inside the player
	if (player->b[0] == NULL)
	{
		player->b[0] = block;
		block->id = 1;
		v_map_b[player->order][0] = block;

	}
	else if (player->b[1] == NULL)
	{
		player->b[1] = block;
		block->id = 2;
		v_map_b[player->order][1] = block;
	}

	// update piece details
	p1->block = true;
	p2->block = true;

	printf("\tA Block %s has been created for the %s player with %s & %s pieces.\n", getBID(block), player->name, getPID(p1), getPID(p2));

	return block;
}

// adding another piece to the block
void addToBlock(Block_p block, Piece_p piece)
{
	if (checkBlock(block, piece))
	{
		printf("Block overloading!!\n");
		return;
	}

	if (block->pieces[2] == NULL)
	{
		block->pieces[2] = piece;
		block->move = 0.34f;	// the move is set to 1/3 or 0.33333 beacuse when creating a block the moving multiple drops by 1/3
	}
	else if (block->pieces[3] == NULL)
	{
		block->pieces[3] = piece;
		block->move = 0.26f;	// the move is set to 1/4 or 0.25 beacuse when creating a block the moving multiple drops by 1/4
	}

	printf("\tThe %s player's piece %s has been added to the block %s(%d/4).\n", getName(block->color), getPID(piece), getBID(block), blockSize(block));

	piece->block = true;

}

// destroy an existing block
void killBlock(Block_p block, Player_p player)
{
	for (short i = 0; i < 4; i++)
		if (block->pieces[i] != NULL)
			(block->pieces[i])->block = false;

	if (player->b[0] == block)
	{
		player->b[0] = NULL;
		v_map_b[player->order][0] = NULL;
	}
	else if (player->b[1] == block)
	{
		player->b[1] = NULL;
		v_map_b[player->order][1] = NULL;
	}

	printf("\tThe block %s by %s Player is broken now.\n", getBID(block), getName(player->color));

	free(block);
	block = NULL;
}