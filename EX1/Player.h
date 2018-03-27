#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
using namespace std;

class Player
{
    Player(int playerNum);
    ~Player(); 
    Player& operator=(const Player& otherPlayer) = delete;			/* deletes the assignment operator - we want a player to be a Non Copyable object */
    
    private:
    int playerNum;
    int *numOfPieces;
    int score;

    int getPlayerNum()const;
    int * getNumOfPieces();
    int getScore()const;

    void setScore(int updateScore);


};