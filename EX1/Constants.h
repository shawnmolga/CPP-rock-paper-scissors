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

#define ROCK 'R'
#define PAPER 'P'
#define SCISSOR 'S'
#define BOMB 'B'
#define JOKER 'J'
#define FLAG 'F'

//TODO : ask what we should do in case of wrong output file.
enum WIN_TYPE {ALL_FLAGS_CAPTURED,ALL_PIECES_EATEN,WRONG_FILE_FORMAT}