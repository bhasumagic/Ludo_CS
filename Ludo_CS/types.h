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
	unsigned char id;
	unsigned char location;
	unsigned char direction;
	unsigned char capture_count;
	float move;

} Piece;

typedef struct
{
	Color color;
	Piece p[4];
	unsigned char current_roll;
	char* name;
	unsigned char count;
} Player;




typedef struct
{
	Piece* pieces[4];
	unsigned char direction;
} Block;

