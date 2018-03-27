#include "Player.h"

Player::Player(int num) : playerNum(num), score(0)
{
    numOfPieces = new int(6);
    numOfPieces={0}; //does it work?
}
int Player::getPlayerNum() const
{
    return playerNum;
}
int * Player::getNumOfPieces() 
{
    return numOfPieces;
}
int Player::getScore() const
{
    return score;
}

Player::~Player()
{
	delete[] numOfPieces;
	return;
}

void Player::setScore(int updateScore)
{
    score = updateScore;
}