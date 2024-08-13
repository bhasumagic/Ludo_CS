#pragma once

#include <stdbool.h>

#define NEWLINE printf("\n")
#define BASE -2
#define HOME -3
#define CLOCKWISE 1
#define ANTICLOCKWISE -1
#define NONE 0

#define LINE for (short i = 0; i < 100; i++) printf("_")
#define HASHLINE for (short i = 0; i < 100; i++) printf("#")


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
	short block_id;
	short location;
	short direction;
	short capture_count;
	short mystery_count;
} Piece;


// defining a new type Block
typedef struct
{
	float move; 
	Color color;
	short location;
	short direction;
} Block;


// defining a new type Player
typedef struct
{
	Piece p[4];
	Block* blocks;
	char* name;
	Color color;
	short order;
	short current_roll;
	short count;
} Player;

extern Piece* v_map[4][4];