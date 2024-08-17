#pragma once

#include "types.h"


short rollDice();

const char* getName(Color color);

const char* getPID(c_Piece_p piece);

short getLoc(short position);



short rolls(Player_p player);

short startRolls(Player_p player);

void toss(Piece_p piece);

void captureP(Piece_p attacking_piece, Piece_p captured_piece);

void setOrder(Player_p p1, Player_p p2, Player_p p3, Player_p p4, Color max_player);

Piece_p getBasePiece(Player_p player);

Block_p getOneBlock(c_Player_p player);



short getHomeDistance(c_Piece_p piece);

short getDestination(c_Player_p player, c_Piece_p piece);

short getBoardPieceCount(c_Player_p player);

short getMax(c_Player_p p1, c_Player_p p2, c_Player_p p3, c_Player_p p4);



Piece_p getPiece(short location);

Block_p getBlock(short location);

short cellStatus(c_Player_p player, short location);

short isRangeClear(c_Player_p player, c_Piece_p piece);