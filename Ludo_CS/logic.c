#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "logic.h"


int game_round = 0;
void go(Player* player);
Piece* v_map[4][4] = { 0 };


// initialize players
Player initPlayer(Color color)
{
	Player player;
	player.color = color;
	player.current_roll = NONE;
	player.count = NONE;

	switch (color)
	{
		case YELLOW: player.name = "yellow";
			break;
		
		case GREEN: player.name = "green";
			break;
		
		case RED: player.name = "red";
			break;
		
		case BLUE: player.name = "blue";
	}

	for (short i = 0; i < 4; i++)
	{
		player.p[i].move = 1.0f;
		player.p[i].color = color;
		player.p[i].block = false;
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].direction = CLOCKWISE;
		player.p[i].capture_count = 0;
	}

	return player;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// rolls the die
short rollDice()
{	
	return (short)rand() % 6 + 1;
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


// rolls the dice for players
short rolls(Player* player)
{
	player->current_roll = rollDice();

	return player->current_roll;
}


// swap the order of players
void setOrder(Player* p1,Player* p2,Player* p3,Player* p4, Color max_player)
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
			if (location == (v_map[i][j])->location && player->color != (v_map[i][j])->color)
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
short getID(short location)
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
		case YELLOW: return "yellow";
		case BLUE: return "blue";
		case RED: return "red";
		case GREEN: return "green";

	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	v_map[1][3] = &(player3->p[3]);

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

				printf("%s player moves piece %d to the starting point.\n", player->name, piece->id);
				printf("%s player now has %hd/4 on pieces on the board and %hd/4 pieces on the base.\n\n", player->name, 1, 3);

				return true;
			}
			else
			{
				printf("%s piece %hd is blocked from moving from BASE to X by %s piece %hd.\n\n", player->name, piece->id, getName(getColor(x_location)), getID(x_location));
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



	}

}


// one game round of a game
bool round(Player* player)
{


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


