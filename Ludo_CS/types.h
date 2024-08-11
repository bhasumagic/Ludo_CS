#pragma once

#include <stdbool.h>


#define NEWLINE printf("\n")
#define BASE -2
#define HOME -3
#define CLOCKWISE 1
#define ANTICLOCKWISE -1


extern Piece* v_map[4][4];

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
	short id;
	short location;
	short direction;
	short capture_count;
} Piece;


typedef struct
{
	Piece p[4];
	char* name;
	Color color;
	short current_roll;
	short count;
} Player;


typedef struct
{
	Piece* pieces[4];
	short direction;
} Block;

