#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "logic.h"


int game_round = 0;


// initialize players
Player initPlayer(Color color)
{
	Player player;
	player.color = color;
	player.current_roll = 0;
	player.count = 0;

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
		player.p[i].id = i + 1;
		player.p[i].location = BASE;
		player.p[i].direction = CLOCKWISE;
		player.p[i].capture_count = 0;
		player.p[i].move = 1.0f;
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


// get the number of pieces in the board
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


// moves a piece of a player
bool move(Player* player)
{
	short base_pieces = getBoardPieceCount(player);

	if (!base_pieces)
	{
		if (rolls(player) == 6)
		{
			Piece* piece = getBasePiece(player);
			piece->location = player->color;


		}
	}






	return false;
}


// Start the game
void start(Player* player1, Player* player2, Player* player3, Player* player4)
{
	srand(time(NULL));
	short max_player;

	do
	{
		rolls(player1);
		printf("Yellow rolls %d\n", player1->current_roll);
		rolls(player2);
		printf("Blue rolls %d\n", player2->current_roll);
		rolls(player3);
		printf("Red rolls %d\n", player3->current_roll);
		rolls(player4);
		printf("Green rolls %d\n", player4->current_roll);

		NEWLINE;

		max_player = getMax(player1, player2, player3, player4);

		if (max_player < 0)
			printf("There is no unique max value. rolling again!\n\n");

	} while (max_player < 0);

	setOrder(player1, player2, player3, player4, max_player);

	printf("%s player has the highest roll and will begin the game.\n", player1->name);
	printf("The order of a single round is %s, %s, %s, and %s.\n", player1->name, player2->name, player3->name, player4->name);

}


// one game round of a game
bool round(Player* player)
{


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


