#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "players.h"
#include "logic.h"
#include "functions.h"


static short bid = 1;

// moves a piece (will also update the map)
bool movePiece(Player_p player, Piece_p piece, short roll)
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

	if (destination < 0)
		return false;

	// checking whether the piece can enter the homestraight
	if (checkPieceToHomeStraight(piece, destination))
	{
		if (piece->direction == CLOCKWISE)
		{
			destination = getLoc(getDestination(piece, roll)) - (short)(piece->color - 2) + 100;
			if (destination > HOME)
				return false;
			if (destination < 100)
				return false;
			piece->location = destination;
			map[current_loc] = 0;
			if (piece->location == HOME)
			{
				printf("\t%s moves piece %s from location %d to HOME!\n", getName(piece->color), getPID(piece), current_loc);
				piece->location = HOME;
				return true;
			}
			printf("\t%s moves piece %s from location %hd to homestraight %hd by %hd units.\n", player->name, getPID(piece), current_loc, piece->location-100, roll);
			return true;
		}
		else
		{
			destination = (short)(piece->color - 2) - getDestination(piece, roll) + 100;
			if (destination > HOME)
				return false;
			if (destination < 100)
				return false;
			piece->location = destination;
			map[current_loc] = 0;
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


	// if the destination is the same as the existing location (not moved)
	if (destination == current_loc)
		// move was unsuccessfull
		return false;
	// if the absolute destination failed
	else if (status == BLOCKED)
	{
		printf("\t%s does not have other pieces in the board to move instead of the blocked piece. Moved the piece %s to square %hd which is the cell before the block.\n", player->name, getPID(piece), destination);

		// checking the cell before block
		status = cellStatus(player->color, destination);

		if (status == AVAILABLE)
		{
			piece->location = destination;
			addPieceToMap(piece, current_loc);
		}
		else if (status == CANCAPTURE)
		{
			Player_p captured_player = getPlayer(getPiece(destination)->color);
			// automatically take the location from the captured piece
			addPieceToMap(captureP(piece, getPiece(destination)), current_loc);
			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
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
		printf("\t%s moves piece %s from location %hd to %hd by %hd units in %s direction.\n", player->name, getPID(piece), current_loc, destination, (short)(roll * piece->move), getDirName(piece->direction));

		if (status == AVAILABLE)
		{
			piece->location = destination;
			addPieceToMap(piece, current_loc);
		}
		else if (status == CANCAPTURE)
		{
			Player_p captured_player = getPlayer(getPiece(destination)->color);
			// automatically take the location from the captured piece
			addPieceToMap(captureP(piece, getPiece(destination)), current_loc);
			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
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
bool moveBlock(Player_p player, Block_p block, short roll)
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

	if (destination < 0)
		return false;

	// checking whether the block can enter the homestraight
	if (checkBlockToHomeStraight(block, destination))
	{
		if (block->direction == CLOCKWISE)
		{
			if ((getLoc(getDestinationB(block, roll)) > (short)(block->color - 2)))
			{
				destination = getLoc(getDestinationB(block, roll)) - (short)(block->color - 2) + 100;
				if (destination > HOME)
					return false;
				if (destination < 100)
					return false;
				block->location = destination;
				map[current_loc] = 0;
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
			if ((getDestinationB(block, roll) < (short)(block->color - 2)))
			{
				destination = getLoc((short)(block->color - 2) - getDestinationB(block, roll) + 100);
				if (destination > HOME)
					return false;
				if (destination < 100)
					return false;
				block->location = destination;
				map[current_loc] = 0;
				for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}
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
		printf("\t%s Moved the block to square %hd which is the cell before the block.\n", player->name, destination);

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
			addBlockToMap(captureBP(block, getPiece(destination)), current_loc);
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
		printf("\t%s moves block %s from location %hd to %hd by %hd units in %s direction.\n", player->name, getBID(block), current_loc, destination, (short)(roll * block->move), getDirName(block->direction));

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
			addBlockToMap(captureBP(block, getPiece(destination)), current_loc);
			for (short i = 0; i < 4; i++){if(block->pieces[i]) block->pieces[i]->location = block->location;}

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", captured_player->name, getBoardPieceCount(captured_player), 4 - getBoardPieceCount(captured_player));
		}
		if (status == CANCAPTUREBLOCK)
		{
			Player_p captured_player = getPlayer(getBlock(destination)->color);
			// automatically take the location from the captured piece
			addBlockToMap(captureBB(block, getBlock(destination)), current_loc);
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

	traffic_count = 0;
	// move was successful
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// game behaviour for the red player
static void goRed(Player_p player)
{
	// try to capture something
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (lookUpPath(piece, player->current_roll) == CANCAPTURE)
			{
				if (movePiece(player, piece, player->current_roll))
				{
					Won(player);
					return;
				}
			}

		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (lookUpPathB(block, player->current_roll) == CANCAPTURE || lookUpPathB(block, player->current_roll) == CANCAPTUREBLOCK)
			{
				if (moveBlock(player, block, player->current_roll))
				{
					Won(player);
					return;
				}
			}
		}
	}


	// when 6 is rolled
	if (player->current_roll == 6)
	{
		// trying to avoid creating blocks
		Piece_p piece = getBoardPiece(player);
		if (piece && canMove(piece))
		{
			if (lookUpPath(piece, player->current_roll) == ADDTOBLOCK || lookUpPath(piece, player->current_roll) == NEWBLOCK)
			{
				if (firstMove(player))
				{
					Won(player);
					return;
				}
			}
			if (movePiece(player, piece, player->current_roll))
			{
				Won(player);
				return;
			}
		}
		// getting a piece out if neccessary
		if (firstMove(player))
		{
			Won(player);
			return;
		}
	}

	// incase no move happens
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (movePiece(player, piece, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (moveBlock(player, block, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}


	traffic_count++;
	printf("\tPassing to the next player\n");
}


// game behaviour for the green player
static void goGreen(Player_p player)
{
	// try to create or expand blocks
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (lookUpPath(piece, player->current_roll) == ADDTOBLOCK || lookUpPath(piece, player->current_roll) == NEWBLOCK)
			{
				if (movePiece(player, piece, player->current_roll))
				{
					Won(player);
					return;
				}
			}

		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (lookUpPathB(block, player->current_roll) == ADDTOBLOCK || lookUpPathB(block, player->current_roll) == MERGEBLOCK)
			{
				if (moveBlock(player, block, player->current_roll))
				{
					Won(player);
					return;
				}
			}
		}
	}

	// keeping the base empty
	if (player->current_roll == 6)
	{
		if (firstMove(player))
		{
			Won(player);
			return;
		}
	}


	// incase no move happens
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (movePiece(player, piece, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (moveBlock(player, block, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}


	traffic_count++;
	printf("\tPassing to the next player\n");
}


// game behaviour for the yellow player
static void goYellow(Player_p player)
{

	// keeping empty base
	if (player->current_roll == 6)
	{
		if (firstMove(player))
		{
			Won(player);
			return;
		}
	}


	// try to capture something
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (lookUpPath(piece, player->current_roll) == CANCAPTURE)
			{
				if (movePiece(player, piece, player->current_roll))
				{
					Won(player);
					return;
				}
			}

		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (lookUpPathB(block, player->current_roll) == CANCAPTURE || lookUpPathB(block, player->current_roll) == CANCAPTUREBLOCK)
			{
				if (moveBlock(player, block, player->current_roll))
				{
					Won(player);
					return;
				}
			}
		}
	}

	// moving the closest piece to HOME
	Piece_p piece = getClosestToHome(player);
	if (piece) 
	{
		if (movePiece(player, piece, player->current_roll))
		{
			Won(player);
			return;
		}
	}


	// incase no move happens
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			if (movePiece(player, piece, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (moveBlock(player, block, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}


	traffic_count++;
	printf("\tPassing to the next player\n");
}


// game behaviour for the blue player
static void goBlue(Player_p player)
{
	// keeping empty base
	if (player->current_roll == 6)
	{
		if (firstMove(player))
		{
			Won(player);
			return;
		}
	}

	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];
		if (canMove(piece))
		{
			if (piece->id > bid++ % 4)
			{
				short s;
				if (decidePath(piece, player->current_roll, &s) == mystery_cell)
				{
					if (piece->direction == ANTICLOCKWISE)
					{
						if (movePiece(player, piece, player->current_roll))
						{
							Won(player);
							return;
						}
					}
				}
				else
				{
					if (movePiece(player, piece, player->current_roll))
					{
						Won(player);
						return;
					}
				}
				
			}
		}
	}

	// incase no move happens
	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece))
		{
			short s;
			if (decidePath(piece, player->current_roll, &s) == mystery_cell)
			{
				if (piece->direction == ANTICLOCKWISE)
				{
					if (movePiece(player, piece, player->current_roll))
					{
						Won(player);
						return;
					}
				}
			}
			else
			{
				if (movePiece(player, piece, player->current_roll))
				{
					Won(player);
					return;
				}
			}
		}
	}
	for (short i = 0; i < 2; i++)
	{
		Block_p block = player->b[i];

		if (block && block->location != HOME)
		{
			if (moveBlock(player, block, player->current_roll))
			{
				Won(player);
				return;
			}
		}
	}


	traffic_count++;
	printf("\tPassing to the next player\n");



}



// game AI switch
static bool go(Player_p player)
{
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
	if (player->current_roll == 6)
	{
		// player->six_count++;

		Piece_p piece = getBasePiece(player);
		if (piece == NULL)
			return false;

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
			addBlockToMap(addToBlock(getBlock(x_location), piece), x_location);

			printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, getBoardPieceCount(player), 4 - getBoardPieceCount(player));

			return true;
		}
		else if (status == BLOCKED)
		{
			printf("\t%s piece %s is blocked from moving from BASE to %hd by %s block %s\n", player->name, getPID(piece), x_location, getName(getBlock(x_location)->color), getBID(getBlock(x_location)));

			return false;
		}
		else
			return false;

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
	// rolling the dice for the player
	rolls(player, true);

	// to track the three consecutive six rolls
	if (player->current_roll == 6)
		player->six_count++;
	else
		player->six_count = 0;


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
