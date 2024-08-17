#pragma once

#include "types.h"

// prototypes of the functions that are called int the main unit

Player initPlayer(Color color);

Block_p initBlock(Player_p player, Piece_p p1, Piece_p p2);

void addToBlock(Block_p block, Piece_p piece);

void killBlock(Block_p block, Player_p player);

void start(Player_p player1, Player_p player2, Player_p player3, Player_p player4); 

void game_round_runner(Player_p player1, Player_p player2, Player_p player3, Player_p player4);