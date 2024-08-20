#include <stdio.h>
#include "types.h"
#include "move.h"
#include "logic.h"
#include "functions.h"


// game behaviour for the yellow player
static void goYellow(Player_p player)
{




}

// game behaviour for the blue player
static void goBlue(Player_p player)
{




}

// game behaviour for the red player
static void goRed(Player_p player)
{




}

// game behaviour for the green player
static void goGreen(Player_p player)
{




}


// game AI switch
static bool go(Player_p player)
{

	rolls(player, true);

	// to track the three consecutive six rolls
	if (player->current_roll == 6)
		player->six_count++;
	else
		player->six_count = 0;


	if (player->six_count > 2)
	{
		player->six_count = 0;
		printf("\t%s player has rolled 6 three times in a row.\n", player->name);

		if (getOneBlock(player) != NULL)
			killBlock(getOneBlock(player), player);
		else
			printf("\tThe %s player has no block to break. passing to the next Player", player->name);
	}



	switch (player->color)
	{
	case YELLOW:return goYellow(player);
	case BLUE:	return goBlue(player);
	case RED:	return goRed(player);
	case GREEN:	return goGreen(player);
	}





}

// the move when all the pieces are at the BASE
static bool firstMove(Player_p player)
{
	if (rolls(player, true) == 6)
	{
		player->six_count++;

		Piece_p piece = getBasePiece(player);
		short x_location = (short)player->color;

		short status = cellStatus(player, x_location);

		if (status == AVAILABLE)	// when there is nothing in the block
		{

			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;

			toss(piece);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, 1, 3);

			return true;
		}
		else if (status == CANCAPTURE)
		{
			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);

			captureP(piece, getPiece(x_location));

			toss(piece);

			printf("\t%s player now has 1/4 on pieces on the board and 3/4 pieces on the base.\n", player->name);

			return true;

		}
		else if (status == BLOCKED)
		{
			printf("\t%s piece %s is blocked from moving from BASE to %hd by %s block %s\n", player->name, getPID(piece), x_location, getName(getBlock(x_location)->color), getBID(getBlock(x_location)));
			printf("\t%s does not have other pieces in the board to move instead of the blocked piece.\n\tIgnoring the throw and moving on to the next player.\n", player->name);

			return false;
		}
	}
	else
	{
		printf("\tpassing to the next player.\n");

		return false;
	}
}


// The main move function
// moves a piece of a player
bool move(Player_p player)
{
	// The first case, where there is no piece on the board
	if (!getBoardPieceCount(player))
		return firstMove(player);

	// second case, when there is one or more pieces in the board
	// This is the main condition tree of the entire game
	else
		return go(player);
}
