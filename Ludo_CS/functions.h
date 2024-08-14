#include "types.h"

#pragma once


short rollDice();

short rolls(Player* player);

short getMax(Player* p1, Player* p2, Player* p3, Player* p4);

void setOrder(Player* p1, Player* p2, Player* p3, Player* p4, Color max_player);

short getBoardPieceCount(Player* player);

Piece* getBasePiece(Player* player);

bool isCellClear(Player* player, short location);

Color getColor(short location);

short getPieceID(short location);

const char* getName(Color color);

short getHomeDistance(Piece* piece);

short isRangeClear(Player* player, Piece* piece);