#pragma once

#include "types.h"

// basic helper functions
short rollDice();
const char* getName(Color color);
const char* getPID(c_Piece_p piece);
const char* getBID(c_Block_p block);
const char* getDirName(short dir);
short getLoc(short position);

// functions that deals with original instances
short rolls(Player_p player, bool out);
void toss(Piece_p piece);
Piece_p captureP(Piece_p attacking_piece, Piece_p captured_piece);
void setOrder(Player_p player1, Player_p player2, Player_p player3, Player_p player4, Color max_player);
Piece_p getBasePiece(Player_p player);
Block_p getOneBlock(c_Player_p player);
Piece_p closestPieceToHome(Player_p player);

// Info functions
short getHomeDistance(c_Piece_p piece);
short getDestination(c_Piece_p piece, short roll);
short getBoardPieceCount(c_Player_p player);
short getMax(c_Player_p player1, c_Player_p player2, c_Player_p player3, c_Player_p player4);
short blockSize(c_Block_p block);
bool checkBlock(c_Block_p block, c_Piece_p piece);

// functions involving virtual maps
Piece_p getPiece(short location);
Block_p getBlock(short location);
short cellStatus(Color color, short location);
void addPieceToMap(Piece_p piece, short last_loacation);
void addBlockToMap(Block_p block, short last_loacation);
short decidePath(Piece_p piece, short roll, short* status, bool out);
short lookUpPath(Piece_p piece, short roll);

// fucntions for blocks
Block_p initBlock(Player_p player, Piece_p p1, Piece_p p2);
Block_p addToBlock(Block_p block, Piece_p piece);
void killBlock(Block_p block, Player_p player);