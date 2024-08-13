#include "types.h"

#pragma once

// prototypes of the functions that are called int the main unit

extern int game_round_count;

Player initPlayer(Color color);

void start(Player* player1, Player* player2, Player* player3, Player* player4);

bool game_round(Player* p1, Player* p2, Player* p3, Player* p4);


