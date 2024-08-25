#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "move.h"
#include "logic.h"
#include "functions.h"


// moves a piece (will also update the map)
static bool movePiece(Player_p player, Piece_p piece, short roll)
{
	// if the piece in the home straight
	if (piece->homestraight)
	{
		if ((piece->location + roll) == HOME)
		{
			printf("\t%s moves piece %s from homestraight location L%hd to HOME!\n", getName(piece->color), getPID(piece), piece->location - 100);
			piece.location = HOME;
			return true;
		}
		else
			return false;
	}

	short current_loc = piece->location;
	short status;
	short destination = decidePath(piece, roll, &status, true);

	if (piece->direction == CLOCKWISE)
	{
		if ((getDestination(piece, roll) > (short)(piece->color - 2)) && piece->distance > 51)
		{
			piece->homestraght = true;
			piece->location = 
		}

	}


	// if the targeted destination failed
	if (status == BLOCKED && destination != current_loc)
	{
		printf("\t%s does not have other pieces in the board to move instead of the blocked piece. Moved the piece to square L%hd which is the cell before the block.", player->name, destination);

		// checking for the adjacent cell before the block

		if (cellStatus(player->color, destination) == AVAILABLE)
		{
			piece->location = destination;
			addPieceToMap(piece, current_loc);
		}
		else if (cellStatus(player->color, destination) == CANCAPTURE)
		{
			// automatically take the location from the captured piece
			addPieceToMap(captureP(piece, getPiece(destination)), current_loc);
		}
		else if (cellStatus(player->color, destination) == NEWBLOCK)
		{
			piece->location = destination;
			addBlockToMap(initBlock(player, piece, getPiece(destination)), current_loc);
		}

		if (piece->direction == CLOCKWISE)
		{
			piece->distance += getDestination(piece, roll) - current_loc;
		}
		else
		{
			piece->distance += current_loc - getDestination(piece, roll);
		}
	}
	// if the targeted destination reached
	else if (destination != current_loc)
	{
		printf("\t%s moves piece %s from location L%hd to L%hd by %hd units in %s direction.\n", getName(piece->color), getPID(piece), current_loc, destination, roll, getDirName(piece->direction));


		if (status == AVAILABLE)
		{
			piece->location = destination;
			addPieceToMap(piece, current_loc);
		}
		else if (status == CANCAPTURE)
		{
			// automatically take the location from the captured piece
			addPieceToMap(captureP(piece, getPiece(destination)), current_loc);
		}
		else if (status == NEWBLOCK)
		{
			piece->location = destination;
			addBlockToMap(initBlock(player, piece, getPiece(destination)), current_loc);
		}
		else if (status == ADDTOBLOCK)
		{
			piece->location = destination;
			addBlockToMap(addToBlock(getBlock(destination), piece), current_loc);
		}

	}

	// if the destination is the same as the existing location
	if (destination == current_loc)
	{
		printf("\t%s does not have other pieces in the board to move instead of the blocked piece.Ignoring the throw and moving on to the next player.\n", getName(piece->color));
		trafiic_count++;
		// move is not successful
		return false;
	}

	// move was successful
	return true;
}

// moves a block (will also update the map)
static bool moveBlock(Player_p player, Block_p block, short roll)
{


	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// game behaviour for the yellow player
static void goYellow(Player_p player)
{
	// yellow's first priority - keep the base empty
	if (player->current_roll == 6)
	{
		if (firstMove(player))
			return;
	}


	// yellow's second priority - capture opponent pieces
	for (short i = 0; i < 4; i++)
	{
		if (lookUpPath(&(player->p[i]), player->current_roll) == CANCAPTURE)
		{
			if (movePiece(player, &(player->p[i]), player->current_roll))
				return;
		}
	}

	// yellow's third priority - moves the piece closest to its home
	movePiece(player, closestPieceToHome(player), player->current_roll);


	// brute forcing other moves if other oves fail

	if (movePiece(player));


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
	// rolling the dice for the player
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

		Block_p block = getOneBlock(player);
		if (block)
		{
			printf("\tThe block %s by %s Player is broken now.\n", getBID(block), getName(player->color));

			short location = block->location;

			if (blockSize(block) == 2)
			{
				Piece_p p1 = block->pieces[0];
				Piece_p p2 = block->pieces[1];

				p1->block = false;
				p2->block = false;

				movePiece(player, p1, 6);
				addPieceToMap(p2, p2->location);
			}
			else if (blockSize(block) == 3)
			{
				Piece_p p1 = block->pieces[0];
				Piece_p p2 = block->pieces[1];
				Piece_p p3 = block->pieces[2];

				p1->block = false;
				p2->block = false;
				p3->block = false;

				movePiece(player, p1, 4);
				movePiece(player, p2, 2);
				addPieceToMap(p2, p3->location);
			}
			else if (blockSize(block) == 4)
			{
				Piece_p p1 = block->pieces[0];
				Piece_p p2 = block->pieces[1];
				Piece_p p3 = block->pieces[2];
				Piece_p p4 = block->pieces[3];

				p1->block = false;
				p2->block = false;
				p3->block = false;
				p4->block = false;

				movePiece(player, p1, 2);
				movePiece(player, p2, 2);
				movePiece(player, p3, 2);
				addPieceToMap(p2, p4->location);
			}

			killBlock(block, player);
		}
		else
			printf("\tThe %s player has no block to break. passing to the next Player", player->name);

		return false;
	}

	switch (player->color)
	{
	case YELLOW:goYellow(player);
	case BLUE:	goBlue(player);
	case RED:	goRed(player);
	case GREEN:	goGreen(player);
	}

	if (player->current_roll == 6)
		return true;
	else
		return false;
}

// the move when all the pieces are at the BASE
static bool firstMove(Player_p player)
{
	// rolling the dice for the player
	if (rolls(player, true) == 6)
	{
		player->six_count++;

		Piece_p piece = getBasePiece(player);
		short x_location = (short)player->color;

		short status = cellStatus(player->color, x_location);

		if (status == AVAILABLE)	// when there is nothing in the block
		{
			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			addPieceToMap(piece, x_location);

			toss(piece);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == CANCAPTURE)
		{
			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);

			captureP(piece, getPiece(x_location));
			addPieceToMap(piece, x_location);

			toss(piece);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == NEWBLOCK)
		{
			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			toss(piece);
			addBlockToMap(initBlock(player, piece, getPiece(x_location)), x_location);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == ADDTOBLOCK)
		{
			printf("\t%s player moves piece %s to the starting point(L%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			toss(piece);
			addBlockToMap(initBlock(player, piece, getPiece(x_location)), x_location);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == BLOCKED)
		{
			printf("\t%s piece %s is blocked from moving from BASE to L%hd by %s block %s\n", player->name, getPID(piece), x_location, getName(getBlock(x_location)->color), getBID(getBlock(x_location)));

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
	// will only run if all the pieces are in the BASE
	if (!getBoardPieceCount(player))
	{
		bool result = firstMove(player);

		if (player->current_roll == 6 && !result)
			printf("\t%s does not have other pieces in the board to move instead of the blocked piece.\n\tIgnoring the throw and moving on to the next player.\n", player->name);

		return result;
	}

	// second case, when there is one or more pieces in the board
	// This is the main condition tree of the entire game
	else
		return go(player);
}
