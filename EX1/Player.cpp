#include "Player.h"

Player::Player(int num) : playerNum(num), score(0), isWinner(false)
{
    numOfPieces = new int(6);
    numOfPieces={0}; //does it work?
	isPlayerTurn = (num == 1 ? true : false);
}
bool Player::getIsWinner() const
{
    return isWinner;
}

void Player::setIsWinner(bool win){
	isWinner = win;
}

bool Player::isLeftMovingPieces()const
{
	for(int i=0;i<6;++i){
		if (i==3 || i==5)
			continue;
		if(numOfPieces[i]!=0){
			return false;
		}
	}
	return true;
}

bool Player::checkPieceOverFlow()
{
  if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
  {
    cout << "Error: a piece type appears in file more than its number"
         << endl;
    return true;
  }
  return false;
}
int Player::getPlayerNum() const
{
    return playerNum;
}

int Player::getScore() const
{
    return score;
}

bool Player::getIsPlayerTurn() const
{
	return isPlayerTurn;
}
void Player::setIsPlayerTurn(bool turn)
{
	isPlayerTurn=turn;
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
