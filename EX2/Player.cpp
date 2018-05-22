#include "Player.h"

Player::Player(int num) :
		playerNum(num), score(0), isWinner(false) {
	numOfPieces = new int[6];
	for (int i = 0; i < 6; i++) {
		numOfPieces[i] = 0;
	}
	isPlayerTurn = (num == 1 ? true : false);
}

bool Player::getIsWinner() const {
	return isWinner;
}

void Player::setIsWinner(bool win) {
	isWinner = win;
}

bool Player::isLeftMovingPieces() const {
	for (int i = 0; i < 6; ++i) {
		if (i == 3 || i == 4 || i == 5)
			continue;
		if (numOfPieces[i] != 0) {
			return true;
		}
	}
	return false;
}

void Player::setNumOfPieces(int index, int value) {
	if (index < 6) {
		numOfPieces[index] = value;
	}
}

int Player::getPlayerNum() const {
	return playerNum;
}

int Player::getScore() const {
	return score;
}

bool Player::getIsPlayerTurn() const {
	return isPlayerTurn;
}
void Player::setIsPlayerTurn(bool turn) {
	isPlayerTurn = turn;
}

Player::~Player() {
	delete[] numOfPieces;
}

void Player::setScore(int updateScore) {
	score = updateScore;
}
