#pragma once

#include <stdbool.h>


#define NEWLINE printf("\n")
#define BASE -2
#define HOME -3
#define CLOCKWISE 1
#define ANTICLOCKWISE -1


typedef enum
{
	YELLOW =0,
	BLUE = 13,
	RED = 26,
	GREEN = 39
} Color;


typedef struct
{
	short id;
	short location;
	short direction;
	short capture_count;
	float move;

} Piece;

typedef struct
{
	Color color;
	Piece p[4];
	short current_roll;
	char* name;
	short count;
} Player;




typedef struct
{
	Piece* pieces[4];
	short direction;
} Block;

