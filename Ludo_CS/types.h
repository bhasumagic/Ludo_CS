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




typedef enum
{
	YELLOW =2,
	BLUE = 15,
	RED = 28,
	GREEN = 41
} Color;


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
} Piece;


typedef struct
{
	Piece p[4];
	char* name;
	Color color;
	short order;
	short current_roll;
	short count;
} Player;


typedef struct
{
	Piece* pieces[4];
	short direction;
} Block;


extern Piece* v_map[4][4];