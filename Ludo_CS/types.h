#pragma once

#include <stdbool.h>

// these are just some intigers just to compare.
#define CLOCKWISE 1
#define NONE 0
#define ANTICLOCKWISE -1

#define ADDTOBLOCK -4
#define NEWBLOCK -5
#define CANCAPTURE -6
#define BLOCKED -7
#define AVAILABLE -8 
#define	MERGEBLOCK -9
#define CANCAPTUREBLOCK -10

#define BASE -2
#define HOME 106
#define NOTBASE 100

#define RANDELL (short)rand()%52

#define NEWLINE printf("\n")
#define LINE printf("\n\033[0m____________________________________________________________________________________________________\n");
#define HLINE printf("####################################################################################################\n");


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
	short id;
	short location;
	short distance;
	short direction;
	short capture_count;
	short mystery_count;
	bool block;
	char effect;
	
} Piece;

typedef Piece* Piece_p;


// defining a new type Block
typedef struct
{
	Piece_p pieces[4];
	float move; 
	Color color;
	short id;
	short location;
	short direction;
} Block;

typedef Block* Block_p;


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



// globals that are used in logics
extern int game_round_count;
extern Player_p players[4];
extern char map[52];
extern short mystery_cell;
extern short traffic_count;
extern bool over;
extern Player_p WPlayer;