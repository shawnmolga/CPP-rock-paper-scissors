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

#define MATERIAL_WEIGHT 8
#define DISCOVERY_WEIGHT 10
#define REVEAL_WEIGHT 3
#define FLAG_SAFTEY_WEUGHT 50
#define DISTANCE_FROM_FLAG_WEIGHT 100
#define DISTANCE_FROM_UNKNOWN_WEIGHT 50

#define PLAYER_ONE 1
#define PLAYER_TWO 2

#define PLAYER_ONE_POSITION_FILENAME "player1.rps_board"
#define PLAYER_TWO_POSITION_FILENAME "player2.rps_board"
#define PLAYER_ONE_MOVE_FILENAME "player1.rps_moves"
#define PLAYER_TWO_MOVE_FILENAME "player2.rps_moves"
#define OUTPUT_FILENAME "rps.output"

#define MAX_NUM_OF_MOVES 100

// move macros
#define ERROR_DURING_MOVE -1
#define JOKER_REP_ERROR -1
#define BAD_FORMAT_ERR -1
#define EOF_MOVE_ERR -2
#define READ_LINE_ERR -3

//position macros
#define BAD_FORMAT_POS_ERR -1
#define READ_LINE_POS_ERR -2
#define NO_BAD_FORMAT_ERR 0 
#define GAME_OVER_SUCC 0
#define MOVE_DONE_SUCC 0 
#define EMPTY_LINE 0 
