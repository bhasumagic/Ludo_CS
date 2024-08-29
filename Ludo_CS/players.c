#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "players.h"
#include "logic.h"
#include "functions.h"


// moves a piece (will also update the map)
static bool movePiece(Player_p player, Piece_p piece, short roll)
{
	short current_loc = piece->location;

	// if the piece in the home straight
	if (piece->location > 99)
	{
		if ((piece->location + roll) == HOME)
		{
			piece->location = HOME;
			printf("\t%s moves piece %s from homestraight location %hd to HOME!\n", getName(piece->color), getPID(piece), current_loc - 100);
			return true;
		}
		else if ((piece->location + roll) < HOME)
		{
			piece->location += roll;
			printf("\t%s moves piece %s from homestraight location %hd to %hd\n", getName(piece->color), getPID(piece), current_loc - 100, piece->location - 100);
			return true;
		}
		else if ((piece->location + roll) > HOME)
		{
			return false;
		}
	}

	short status;
	short destination = decidePath(piece, roll, &status);
	short absolute_destination = getLoc(getDestination(piece, roll));
	// adding the distance to the piece
	piece->distance += abs(getDestination(piece, roll) - current_loc);

	// checking whether the piece can enter the homestraight
	if (piece->distance > 51 && piece->capture_count > 0)
	{
		if (piece->direction == CLOCKWISE)
		{
			if ((getLoc(getDestination(piece, roll)) > (short)(piece->color - 2)))
			{
				destination = getLoc(getDestination(piece, roll)) - (short)(piece->color - 2) + 100;
				if (destination > HOME)
					return false;
				piece->location = destination;
				if (piece->location == HOME)
				{
					printf("\t%s moves piece %s from location %d to HOME!\n", getName(piece->color), getPID(piece), current_loc);
					piece->location = HOME;
					return true;
				}
				printf("\t%s moves piece %s from location %hd to homestraight %hd by %hd units.\n", player->name, getPID(piece), current_loc, piece->location-100, roll);
				return true;
			}
		}
		else
		{
			if ((getDestination(piece, roll) < (short)(piece->color - 2)))
			{
				destination = getLoc(getDestination(piece, roll)) - (short)(piece->color - 2) + 100;
				if (destination > HOME)
					return false;
				piece->location = destination;
				if (piece->location == HOME)
				{
					printf("\t%s moves piece %s from location %d to HOME!\n", getName(piece->color), getPID(piece), current_loc);
					piece->location = HOME;
					return true;
				}
				printf("\t%s moves piece %s from location %hd to homestraight %hd by %hd units.\n", player->name, getPID(piece), current_loc, piece->location-100, roll);
				return true;
			}
		}
	}


	// if the destination is the same as the existing location (not moved)
	if (destination == current_loc)
		// move was unsuccessfull
		return false;
	// if the absolute destination failed
	else if (status == BLOCKED)
	{
		printf("\t%s does not have other pieces in the board to move instead of the blocked piece. Moved the piece to square %hd which is the cell before the block.", player->name, destination);

		// checking the cell before block
		status = cellStatus(player->color, destination);

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
	}
	// if the targeted destination reached
	else
	{
		printf("\t%s moves piece %s from location %hd to %hd by %hd units in %s direction.\n", player->name, getPID(piece), current_loc, destination, roll, getDirName(piece->direction));

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


	// move was successful
	return true;
}

// moves a block (will also update the map)
static bool moveBlock(Player_p player, Block_p block, short roll)
{
	short current_loc = block->location;

	// if the block in the home straight
	if (block->location > 99)
	{
		if ((block->location + roll) == HOME)
		{
			block->location = HOME;
			printf("\t%s moves block %s from homestraight location %hd to HOME!\n", getName(block->color), getBID(block), current_loc - 100);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
			return true;
		}
		else if ((block->location + roll) < HOME)
		{
			block->location += roll;
			printf("\t%s moves block %s from homestraight location %hd to %hd\n", getName(block->color), getBID(block), current_loc - 100, block->location - 100);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
			return true;
		}
		else if ((block->location + roll) > HOME)
		{
			return false;
		}
	}

	short status;
	short destination = decidePathB(block, roll, &status);
	short absolute_destination = getLoc(getDestinationB(block, roll));
	// adding the distances
	for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->distance += (abs(getDestinationB(block, roll)) - current_loc);}

	// checking whether the block can enter the homestraight
	if (checkBlockToHomeStraight(block))
	{
		if (block->direction == CLOCKWISE)
		{
			if ((getLoc(getDestinationB(block, roll)) > (short)(block->color - 2)))
			{
				destination = getLoc(getDestination(block, roll)) - (short)(block->color - 2) + 100;
				if (destination > HOME)
					return false;
				block->location = destination;
				for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
				if (block->location == HOME)
				{
					block->location = HOME;
					printf("\t%s moves block %s from location %d to HOME!\n", getName(block->color), getBID(block), current_loc);
					return true;
				}
				printf("\t%s moves block %s from location %hd to homestraight %hd by %hd units.\n", player->name, getBID(block), current_loc, block->location-100, roll);
				return true;
			}
		}
		else
		{
			if ((getDestination(block, roll) < (short)(block->color - 2)))
			{
				destination = getLoc((short)(block->color - 2) - getDestination(block, roll) + 100);
				if (destination > HOME)
					return false;
				block->location = destination;
				for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location == block->location;}
				if (block->location == HOME)
				{
					block->location = HOME;
					printf("\t%s moves block %s from location %d to HOME!\n", getName(block->color), getBID(block), current_loc);
					return true;
				}
				printf("\t%s moves piece %s from location %hd to homestraight %hd by %hd units.\n", player->name, getBID(block), current_loc, block->location-100, roll);
				return true;
			}
		}
	}

	// if the destination is the same as the existing location (not moved)
	if (destination == current_loc)
		// move was unsuccessfull
		return false;
	// if the absolute destination failed
	else if (status == BLOCKED)
	{
		printf("\t%s Moved the block to square %hd which is the cell before the block.", player->name, destination);

		// checking the cell before block
		status = cellStatus(player->color, destination);

		if (status == AVAILABLE)
		{
			block->location = destination;
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
			addBlockToMap(block, current_loc);
		}
		else if (status == CANCAPTURE)
		{
			Player_p captured_player = getPlayer(getPiece(destination)->color);
			// automatically take the location from the captured piece
			addPieceToMap(captureBP(block, getPiece(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
		}
		else if (status == ADDTOBLOCK)
		{
			block->location = destination;
			addBlockToMap(addToBlock(block, getPiece(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
		}
	}
	// if the targeted destination reached
	else
	{
		printf("\t%s moves block %s from location %hd to %hd by %hd units in %s direction.\n", player->name, getBID(block), current_loc, destination, roll, getDirName(block->direction));

		if (status == AVAILABLE)
		{
			block->location = destination;
			addBlockToMap(block, current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
		}
		if (status == CANCAPTURE)
		{
			Player_p captured_player = getPlayer(getPiece(destination)->color);
			// automatically take the location from the captured piece
			addPieceToMap(captureBP(block, getPiece(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
		}
		if (status == CANCAPTUREBLOCK)
		{
			Player_p captured_player = getPlayer(getBlock(destination)->color);
			// automatically take the location from the captured piece
			addPieceToMap(captureBB(block, getBlock(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
		}
		if (status == ADDTOBLOCK)
		{
			block->location = destination;
			addBlockToMap(addToBlock(block, getPiece(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
		}
		if (status == MERGEBLOCK)
		{
			block->location = destination;
			addBlockToMap(mergeBlock(block, getBlock(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
		}
	}


	// move was successful
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// game behaviour for the yellow player
static void goYellow(Player_p player)
{
	// yellow's first priority - keep the base empty


	// yellow's second priority - capture opponent pieces
	
	// checking for pieces


	// checking for blocks


	// yellow's third priority - moves the piece closest to its home




	// brute forcing other moves if other oves fail

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


	// breaking blocks of the player if the 6 was rolled 3 times
	if (player->six_count > 2)
	{
		player->six_count = 0;
		printf("\t%s player has rolled 6 three times in a row.\n", player->name);

		Block_p block = getOneBlock(player);
		if (block)
		{
			printf("\tThe block %s by %s Player is broken now.\n", getBID(block), getName(player->color));

			short location = block->location;

			// breaking block process yet to write

			killBlock(block, player);
		}
		else
			printf("\tThe %s player has no block to break. passing to the next Player", player->name);

		return false;
	}

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
			printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			addPieceToMap(piece, x_location);

			toss(piece);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == CANCAPTURE)
		{
			printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);

			captureP(piece, getPiece(x_location));
			addPieceToMap(piece, x_location);

			toss(piece);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == NEWBLOCK)
		{
			printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			toss(piece);
			addBlockToMap(initBlock(player, piece, getPiece(x_location)), x_location);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == ADDTOBLOCK)
		{
			printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);
			piece->location = x_location;
			toss(piece);
			addBlockToMap(initBlock(player, piece, getPiece(x_location)), x_location);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == BLOCKED)
		{
			printf("\t%s piece %s is blocked from moving from BASE to %hd by %s block %s\n", player->name, getPID(piece), x_location, getName(getBlock(x_location)->color), getBID(getBlock(x_location)));

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
		return firstMove(player);
	}

	// second case, when there is one or more pieces in the board
	// This is the main condition tree of the entire game
	else
		return go(player);
}
