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

	// checking whether the piece can enter the homestraight
	if (checkPieceToHomeStraight(piece, destination))
	{
		if (piece->direction == CLOCKWISE)
		{
			destination = getLoc(getDestination(piece, roll)) - (short)(piece->color - 2) + 100;
			if (destination > HOME)
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


	if (Won(player))
	{
		WPlayer = player;
	}
	traffic_count = 0;
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


	if (Won(player))
	{
		WPlayer = player;
	}
	traffic_count = 0;
	// move was successful
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// game behaviour for the yellow player
static void goYellow(Player_p player)
{
	// yellow's first priority - keep the base empty
	if (player->current_roll == 6)
	{
		if(firstMove(player))
			return;
	}

	for (short i = 0; i < 4; i++)
	{
		if (lookUpPath(&player->p[i], player->current_roll) == CANCAPTURE)
		{
			if (movePiece(player, &player->p[i], player->current_roll))
				return;
		}
	}
	for (short i = 0; i < 2; i++)
	{
		if (player->b[i] && (lookUpPathB(player->b[i], player->current_roll) == CANCAPTURE || lookUpPathB(player->b[i], player->current_roll) == CANCAPTUREBLOCK))
		{
			if (moveBlock(player, player->b[i], player->current_roll))
				return;
		}
	}

	Piece_p p = getBoardPiece(player);
	if (p)
	{
		short homedistance = getHomeDistance(p);
		for (short i = 0; i < 4; i++)
		{
			if (canMove(&player->p[i]))
			{
				if (getHomeDistance(&player->p[i]) < homedistance)
				{
					p = &player->p[i];
					homedistance = getHomeDistance(p);
				}
			}
		}
	}


	if (p && movePiece(player, p, player->current_roll))
		return;

	// non priority moves
	// one piece will move
	for (short i = 0; i < 4; i++)
	{
		if (canMove(&player->p[i]))
			if (movePiece(player, &player->p[i], player->current_roll))
				return;
	}

	// one block will move
	if (player->b[0] && moveBlock(player, player->b[0], player->current_roll))
		return;
	if (player->b[1] && moveBlock(player, player->b[1], player->current_roll))
		return;


	// if the move is not successful
	printf("\tThere are no available moves. Passing.\n");
	traffic_count++;

	
}

// game behaviour for the blue player
static void goBlue(Player_p player)
{
	// this player's prorities cannot be described point by point

	
	if (player->current_roll == 6)
	{
		if (firstMove(player))
			return;
	}

	for (short i = 0; i < 4; i++)
	{
		Piece_p piece = &player->p[i];

		if (canMove(piece) && piece->id >= bid % 4)
		{
			short s;
			short loc = decidePath(piece, player->current_roll, &s);

			if (loc == mystery_cell)
			{
				if (piece->direction == ANTICLOCKWISE)
				{
					if (movePiece(player, piece, player->current_roll))
					{
						bid++;
						return;
					}
				}
			}
			else
			{
				if(movePiece(player, piece, player->current_roll))
				{
					bid++;
					return;
				}
			}

		}
	}

	// non priority moves
	// one piece will move
	for (short i = 0; i < 4; i++)
	{
		if (canMove(&player->p[i]))
			if(movePiece(player, &player->p[i], player->current_roll))
			{
				bid = player->p[i].id + 1;
				return;
			}
	}

	// one block will move
	if (player->b[0] && moveBlock(player, player->b[0], player->current_roll))
		return;
	if (player->b[1] && moveBlock(player, player->b[1], player->current_roll))
		return;


	// move was unsuccessful
	printf("\tThere are no available moves. Passing.\n");
	traffic_count++;
}

// game behaviour for the red player
static void goRed(Player_p player)
{

	// Red's first priority - capture opponent pieces, even if the roll is a six
	// checking the available captures for pieces
	for (short i = 0; i < 4; i++)
	{
		if (!player->p[i].block && lookUpPath(&player->p[i], player->current_roll) == CANCAPTURE)
			if(movePiece(player, &player->p[i], player->current_roll))
				return;
	}
	// checking the available captures for blocks, if exists
	for (short i = 0; i < 2; i++)
	{
		if (player->b[i] && (lookUpPathB(player->b[i], player->current_roll) == CANCAPTURE || lookUpPathB(player->b[i], player->current_roll) == CANCAPTUREBLOCK))
			if(moveBlock(player, player->b[i], player->current_roll))
				return;
	}

	for (short i = 0; i < 4; i++)
	{
		if (canMove(&player->p[i]))
		{
			short s;
			short loc = decidePath(&player->p[i], player->current_roll, &s);
			if (s != BLOCKED)
				if (movePiece(player, &player->p[i], player->current_roll))
					return;

		}
	}

	// Red's second priority - taking a piece out if there is no move to capture anything
	if (player->current_roll == 6)
	{
		// if it'll create a block by taking a piece out
		if (cellStatus(player->color, (short)(player->color)) == NEWBLOCK)
		{
			// then red will move one piece that is already on the board
			for (short i = 0; i < 4; i++)
			{
				if (!player->p[i].block && canMove(&player->p[i]))
					if(movePiece(player, &player->p[i], player->current_roll))
						return;
			}

			// then red will move one block that is already on the board
			if (player->b[0] && moveBlock(player, player->b[0], player->current_roll))
				return;
			if (player->b[1] && moveBlock(player, player->b[1], player->current_roll))
				return;	
		}
		// after all red will take a piece out even it makes a block
		if(firstMove(player))
			return;
	}


	// non priority moves
	// one piece will move
	for (short i = 0; i < 4; i++)
	{
		if (canMove(&player->p[i]))
			if(movePiece(player, &player->p[i], player->current_roll))
				return;
	}

	// one block will move
	if (player->b[0] && moveBlock(player, player->b[0], player->current_roll))
		return;
	if (player->b[1] && moveBlock(player, player->b[1], player->current_roll))
		return;


	// if the move is not successful
	printf("\tThere are no available moves. Passing.\n");
	traffic_count++;
}

// game behaviour for the green player
static void goGreen(Player_p player)
{
	// green's first priority - make blocks
	for (short i = 0; i < 4; i++)
	{
		short stat;
		decidePath(&player->p[i], player->current_roll, &stat);

		if (stat != BLOCKED)
		{
			short res = lookUpPath(&player->p[i], player->current_roll);
			if (canMove(&player->p[i]) && (res == NEWBLOCK || res == ADDTOBLOCK))
				if(movePiece(player, &player->p[i], player->current_roll))
					return;
		}
			
	}
	for (short i = 0; i < 2; i++)
	{
		if (player->b[i] && (lookUpPathB(player->b[i], player->current_roll) == ADDTOBLOCK || lookUpPathB(player->b[i], player->current_roll) == MERGEBLOCK))
			if(moveBlock(player, player->b[i], player->current_roll))
				return;
	}


	// green's first priority - keep the base empty
	if (player->current_roll == 6)
	{
		if(firstMove(player))
			return;
	}

	// capturing a piece (mandatory)
	for (short i = 0; i < 4; i++)
	{
		if (!player->p[i].block && player->p[i].capture_count < 1 && lookUpPath(&player->p[i], player->current_roll) == CANCAPTURE)
			if(movePiece(player, &player->p[i], player->current_roll))
				return;
	}
	for (short i = 0; i < 2; i++)
	{
		if (player->b[i] && player->b[i]->pieces[0]->capture_count < 1 && (lookUpPathB(player->b[i], player->current_roll) == CANCAPTURE || lookUpPathB(player->b[i], player->current_roll) == CANCAPTUREBLOCK))
			if(moveBlock(player, player->b[i], player->current_roll))
				return;
	}

	// non priority moves
	// one piece will move
	for (short i = 0; i < 4; i++)
	{
		if (canMove(&player->p[i]))
			if(movePiece(player, &player->p[i], player->current_roll))
				return;
	}

	// one block will move
	if (player->b[0] && moveBlock(player, player->b[0], player->current_roll))
		return;
	if (player->b[1] && moveBlock(player, player->b[1], player->current_roll))
		return;


	// move was unsuccessful
	printf("\tThere are no available moves. Passing.\n");
	traffic_count++;
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

			// killBlock(block, player);
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
