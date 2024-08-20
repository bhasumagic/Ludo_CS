#pragma once

#include "types.h"

// prototypes of the functions that are called int the main unit

Player initPlayer(Color color);

void start(Player_p player1, Player_p player2, Player_p player3, Player_p player4); 

void game_round_runner(Player_p player1, Player_p player2, Player_p player3, Player_p player4);