#pragma once

#include <stdbool.h>

#define NEWLINE printf("\n")

// these are just some intigers just to compare.
#define CLOCKWISE 1
#define NONE 0
#define ANTICLOCKWISE -1
#define BASE -2
#define HOME -3
#define ADDTOBLOCK -4
#define NEWBLOCK -5
#define CANCAPTURE -6
#define BLOCKED -7
#define AVAILABLE -8


#define LINE printf("\n____________________________________________________________________________________________________\n");
#define HASHLINE printf("\n####################################################################################################\n");


// defining a new type Color
typedef enum
{
	YELLOW =2,
	BLUE = 15,
	RED = 28,
	GREEN = 41
} Color;


// defining a new type Piece
typedef struct
{
	float move;
	Color color;
	bool block;
	short id;
	short location;
	short direction;
	short capture_count;
	short mystery_count;
} Piece;

typedef Piece* Piece_p;
typedef const Piece* c_Piece_p;


// defining a new type Block
typedef struct
{
	Piece_p pieces[4];
	char* name;
	float move; 
	Color color;
	short location;
	short direction;
} Block;

typedef Block* Block_p;
typedef const Block* c_Block_p;


// defining a new type Player
typedef struct
{
	Piece p[4];
	Block_p b[2];
	char* name;
	Color color;
	short order;
	short current_roll;
	short six_count;
} Player;

typedef Player* Player_p;
typedef const Player* c_Player_p;



// globals that are used in logics
extern int game_round_count;
extern Piece_p v_map_p[4][4];
extern Block_p v_map_b[4][2];