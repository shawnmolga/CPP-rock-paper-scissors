#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
using namespace std;

class Player
{
	public:
	Player(int playerNum);
	~Player();
	Player& operator=(const Player& otherPlayer) = delete;	/* deletes the assignment operator - we want a player to be a Non Copyable object */

	int * numOfPieces;

	int getPlayerNum()const;
	int getScore()const;
	bool getIsWinner() const;
	bool getIsPlayerTurn() const;
	void setIsWinner(bool isWinner);
	void setScore(int updateScore);
	bool isLeftMovingPieces()const;
	void setIsPlayerTurn(bool turn );
	void setNumOfPieces(int index,int value);

private:
	int playerNum;
	int score;
	bool isPlayerTurn;
	bool isWinner;
};
