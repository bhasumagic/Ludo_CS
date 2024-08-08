#pragma once


extern int game_round;

Player initPlayer(Color color);

void start(Player* player1, Player* player2, Player* player3, Player* player4);

void round(Player* player);



