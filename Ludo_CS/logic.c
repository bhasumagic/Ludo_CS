#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"
#include "types.h"
#include "functions.h"


void go(Player* player);		// the prototype of the "go()" function in the "gameAI.c"


int game_round_count = 0;
Piece_p v_map_p[4][4] = { 0 };	// virtual map for pieces
Block_p v_map_b[4][2] = { 0 };	// virtual map for blocks


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// special functions

// initialize players
Player initPlayer(Color color)
{
	Player player;
	player.b[0] = NULL;
	player.b[1] = NULL;
	player.color = color;
	player.order = NONE;
	player.current_roll = NONE;
	player.six_count = NONE;
	player.name = (char*)getName(color);

	for (short i = 0; i < 4; i++)
	{
		player.p[i].move = 1.0f;
		player.p[i].color = color;
		player.p[i].block = false;
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].direction = NONE;
		player.p[i].capture_count = 0;
		player.p[i].mystery_count = 0;
	}

	printf("The %s player has four (04) pieces named %s, %s, %s, and %s.\n", player.name, getPID(&player.p[0]), getPID(&player.p[1]), getPID(&player.p[2]), getPID(&player.p[3]) );

	return player;
}


// create a block
Block_p initBlock(Player_p player, Piece_p p1, Piece_p p2)
{
	Block_p block = (Block_p)malloc(sizeof(Block));

	// if there is a allocation error
	if (block == NULL)
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
		block->name = "B1";
	}
	else if (player->b[1] == NULL)
	{
		player->b[1] = block;
		block->name = "B2";
	}

	// update piece details
	p1->block = true;
	p2->block = true;

	return block;
}


// adding another piece to the block
void addToBlock(Block_p block, Piece_p piece)
{
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

	piece->block = true;
}


// destroy an existing block
void killBlock(Block_p block, Player_p player)
{
	for (short i = 0; i < 2; i++)
		if (block->pieces[i] != NULL)
			(block->pieces[i])->block = false;
	
	if (player->b[0] != NULL)
		player->b[0] = NULL;
	else if(player->b[1] != NULL)
		player->b[1] = NULL;

	free(block);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main functions

// Start the game
void start(Player_p player1, Player_p player2, Player_p player3, Player_p player4)
{
	srand((unsigned int)time(NULL));
	short max_player;

	do
	{
		printf("Yellow rolls %d\n", startRolls(player1));
		printf("Blue rolls %d\n", startRolls(player2));
		printf("Red rolls %d\n", startRolls(player3));
		printf("Green rolls %d\n", startRolls(player4));

		NEWLINE;

		max_player = getMax(player1, player2, player3, player4);

		if (max_player < 0)
			printf("\tThere is no unique max value. rolling again!\n\n");

	} while (max_player < 0);

	setOrder(player1, player2, player3, player4, max_player);

	printf("%s player has the highest roll and will begin the game.\n", player1->name);
	printf("The order of a single round is %s, %s, %s, and %s.\n", player1->name, player2->name, player3->name, player4->name);


	// maping the virtual map for pieces
	
	v_map_p[0][0] = &(player1->p[0]);
	v_map_p[0][1] = &(player1->p[1]);
	v_map_p[0][2] = &(player1->p[2]);
	v_map_p[0][3] = &(player1->p[3]);
		 
	v_map_p[1][0] = &(player2->p[0]);
	v_map_p[1][1] = &(player2->p[1]);
	v_map_p[1][2] = &(player2->p[2]);
	v_map_p[1][3] = &(player2->p[3]);
		 
	v_map_p[2][0] = &(player3->p[0]);
	v_map_p[2][1] = &(player3->p[1]);
	v_map_p[2][2] = &(player3->p[2]);
	v_map_p[2][3] = &(player3->p[3]);
		 
	v_map_p[3][0] = &(player4->p[0]);
	v_map_p[3][1] = &(player4->p[1]);
	v_map_p[3][2] = &(player4->p[2]);
	v_map_p[3][3] = &(player4->p[3]);


	// maping the virtual map for blocks

	v_map_b[0][0] = player1->b[0];
	v_map_b[0][1] = player1->b[1];
		  
	v_map_b[1][0] = player2->b[0];
	v_map_b[1][1] = player2->b[1];
		  
	v_map_b[2][0] = player3->b[0];
	v_map_b[2][1] = player3->b[1];
		  
	v_map_b[3][0] = player4->b[0];
	v_map_b[3][1] = player4->b[1];

}


// moves a piece of a player
bool move(Player_p player)
{
	// The first case, where there is no piece on the board
	// This condition tree will only run one time for each player, that means only 4 times per game.
	if (!getBoardPieceCount(player))
	{
		if (rolls(player) == 6)
		{
			player->six_count++;

			Piece_p piece = getBasePiece(player);
			short x_location = (short)player->color;
			
			short status = cellStatus(player, x_location);

			if (status == AVAILABLE)	// when there is nothing in the block
			{

				printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);
				piece->location = x_location; 

				toss(piece);

				printf("\t%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, 1, 3);

				return true;
			}
			else if (status == CANCAPTURE)
			{
				printf("\t%s player moves piece %s to the starting point(%hd).\n", player->name, getPID(piece), x_location);
				piece->location = x_location;

				captureP(piece, getPiece(x_location));

				toss(piece);

				printf("\t%s player now has 1/4 on pieces on the board and 3/4 pieces on the base.\n", player->name);

				return true;

			}
			else if (status == BLOCKED)
			{
				printf("%s piece %s is blocked from moving from BASE to %hd by %s block %s\n", player->name, getPID(piece), x_location, getName(getBlock(x_location)->color), getBlock(x_location)->name);
				printf("%s does not have other pieces in the board to move instead of the blocked piece. Ignoring the throw and moving on to the next player.\n", player->name);

				return false;
			}
		}
		else
		{
			printf("\tpassing to the next player.\n");

			return false;
		}
	}



	// second case, when there is at least one piece in the board
	// This is the main condition tree of the entire game

	rolls(player);

	// to track the three consecutive six rolls
	if (player->current_roll == 6) player->six_count++;
	else player->six_count = 0;
	
	if (player->six_count > 2)
	{
		player->six_count = 0;
		if (getOneBlock(player) != NULL)
			killBlock(getOneBlock(player), player);
		else
			printf("\tThe %s player has no block to break. passing to the next Player", player->name);
	}


	go(player);

	return false;	// default return value
}


// one game round of a game
void game_round_runner(Player_p player1, Player_p player2, Player_p player3, Player_p player4)
{
	game_round_count++;
	printf(">>> Game round - %d\n", game_round_count);


	while (move(player1));
	while (move(player2));
	while (move(player3));
	while (move(player4));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////