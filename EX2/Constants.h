#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
using namespace std;

#define ROWS 10
#define COLS 10

#define ROCKS_NUM 2
#define PAPERS_NUM 5
#define SCISSORS_NUM 1
#define BOMBS_NUM 2
#define JOKERS_NUM 2
#define FLAGS_NUM 1
#define TOTAL_PIECES_NUM (ROCKS_NUM+PAPERS_NUM+SCISSORS_NUM+BOMBS_NUM+JOKERS_NUM+FLAGS_NUM)

#define ROCK_SCORE 1
#define PAPER_SCORE 1
#define SCISSOR_SCORE 1
#define BOMB_SCORE 3
#define JOKER_SCORE 9
#define FLAG_SCORE 100

#define ROCK 'R'
#define PAPER 'P'
#define SCISSOR 'S'
#define BOMB 'B'
#define JOKER 'J'
#define FLAG 'F'

#define PLAYER_ONE 1
#define PLAYER_TWO 2

#define PLAYER_ONE_POSITION_FILENAME "player1.rps_board"
#define PLAYER_TWO_POSITION_FILENAME "player2.rps_board"
#define PLAYER_ONE_MOVE_FILENAME "player1.rps_moves"
#define PLAYER_TWO_MOVE_FILENAME "player2.rps_moves"
#define OUTPUT_FILENAME "rps.output"
