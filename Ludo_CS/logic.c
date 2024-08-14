#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"
#include "types.h"
#include "functions.h"


void go(Player* player);	// the prototype of the "go()" function in the "gameAI.c"

int game_round_count = 0;
Piece* v_map[4][4] = { 0 };

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
	player.count = NONE;
	player.name = (char*)getName(color);

	for (short i = 0; i < 4; i++)
	{
		player.p[i].move = 1.0f;
		player.p[i].color = color;
		player.p[i].block = false;
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].direction = CLOCKWISE;
		player.p[i].capture_count = 0;
		player.p[i].mystery_count = 0;
	}

	return player;
}


// create a block
Block* initBlock(Player* player, Piece* p1, Piece* p2)
{
	Block* block = (Block*)malloc(sizeof(Block));

	block->color = player->color;

	if (p1->direction != p2->direction)
		block->direction = (getHomeDistance(p1) > getHomeDistance(p2)) ? p1->direction : p2->direction;
	else
		block->direction = p1->direction;

	block->move = 0.5;	// the move is set to 1/2 or 0.5 beacuse when creating a block the moving multiple drops by 1/2

	if (player->b[0] != NULL)
		player->b[0] = block;
	else
		player->b[1] = block;

	p1->block = true;
	p2->block = true;

	return block;
}


// adding another piece to the block
Block* addToBlock(Block* existing_block)
{
	free(existing_block);



}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main functions

// Start the game
void start(Player* player1, Player* player2, Player* player3, Player* player4)
{
	srand((unsigned int)time(NULL));
	short max_player;

	do
	{
		printf("Yellow rolls %d\n", rolls(player1));
		printf("Blue rolls %d\n", rolls(player2));
		printf("Red rolls %d\n", rolls(player3));
		printf("Green rolls %d\n", rolls(player4));

		NEWLINE;

		max_player = getMax(player1, player2, player3, player4);

		if (max_player < 0)
			printf("There is no unique max value. rolling again!\n\n");

	} while (max_player < 0);

	setOrder(player1, player2, player3, player4, max_player);

	printf("%s player has the highest roll and will begin the game.\n", player1->name);
	printf("The order of a single round is %s, %s, %s, and %s.\n", player1->name, player2->name, player3->name, player4->name);


	// maping the virtual map

	v_map[0][0] = &(player1->p[0]);
	v_map[0][1] = &(player1->p[1]);
	v_map[0][2] = &(player1->p[2]);
	v_map[0][3] = &(player1->p[3]);

	v_map[1][0] = &(player2->p[0]);
	v_map[1][1] = &(player2->p[1]);
	v_map[1][2] = &(player2->p[2]);
	v_map[1][3] = &(player2->p[3]);

	v_map[2][0] = &(player3->p[0]);
	v_map[2][1] = &(player3->p[1]);
	v_map[2][2] = &(player3->p[2]);
	v_map[2][3] = &(player3->p[3]);

	v_map[3][0] = &(player4->p[0]);
	v_map[3][1] = &(player4->p[1]);
	v_map[3][2] = &(player4->p[2]);
	v_map[3][3] = &(player4->p[3]);

}


// moves a piece of a player
bool move(Player* player)
{
	short board_pieces = getBoardPieceCount(player);

	// The first case, where there is no piece on the board
	if (!board_pieces)
	{
		if (rolls(player) == 6)
		{
			printf("\n%s player rolled %hd.\n", player->name, player->current_roll);
			Piece* piece = getBasePiece(player);
			short x_location = (short)player->color;

			if (isCellClear(player, x_location))
			{
				piece->location = x_location;
				player->count++;

				printf("%s player moves piece %d to the starting point(%hd).\n", player->name, piece->id, x_location);
				printf("%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n", player->name, 1, 3);


				if (rollDice() % 2)
				{
					printf("The coin toss was head, %s moves piece %hd will move in clockwise direction.\n", player->name, piece->id);
					return true;
				}
				else
				{
					printf("The coin toss was tail, %s moves piece %hd will move in counterclockwise direction.\n", player->name, piece->id);
					return true;
				}


			}
			else
			{
				printf("%s piece %hd is blocked from moving from BASE to X by %s block %hd.\n", player->name, piece->id, getName(getColor(x_location)), getPieceID(x_location));
				return false;
			}
		}
		else
		{
			printf("\n%s player rolled %hd. passing to the next player.\n", player->name, player->current_roll);
			return false;
		}
	}



	// second case
	// when there is at least one piece in the board
	if (player->count < 3)
	{
		printf("\n%s player rolled %hd.\n", player->name, rolls(player));
		if (player->current_roll == 6) player->count++;
		else player->count = 0;

		go(player);
	}
	else
	{
		player->count = 0;
	}
	return false;

}


// one game round of a game
bool game_round(Player* p1, Player* p2, Player* p3, Player* p4)
{
	game_round_count++;
	printf(">>> Game round - %d\n", game_round_count);


	while (move(p1));
	while (move(p2));
	while (move(p3));
	while (move(p4));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


