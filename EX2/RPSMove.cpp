/*
 * RPSMove.cpp
 *
 *  Created on: 28 Apr. 2018
 *      Author: shawn
 */


#include "RPSMove.h"

RPSMove::RPSMove(RPSpoint from, RPSpoint to) : from(from), to(to){}

RPSMove::~RPSMove(){}
const Point& RPSMove::getFrom() const{
	return from;
}

const Point& RPSMove::getTo() const{
	return to;
}

void RPSMove::setFrom(RPSpoint point) {
	from = point;
}

void RPSMove::setTo(RPSpoint point) {
	to = point;
}

bool RPSMove::isLegalMove(RPSBoard gameBoard, unique_ptr<Move> &move, bool isPlayer1) {
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getFrom().getX();
	int to_y = move->getFrom().getY();

	if ((move->getFrom().getX() < 1 || from_x > ROWS)
		|| (to_x < 1 || to_x > ROWS) || (from_y < 1 || from_y > COLS)
		|| (to_y < 1 || to_y > COLS)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y) {
		cout << "Error: user MUST move one piece" << endl;
		return false;
	}

	if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0) {
		cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece()))
		|| (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece()))) {
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB
		|| toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG) {
		cout << "Error: flag/bomb piece is not allowed to move" << endl;
		return false;
	}

	if (to_x == from_x + 1 || to_x == from_x - 1) {
		if (to_y != from_y) {
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right "
				<< endl;
			return false;
		}
	}
	else if (to_y == from_y + 1 || to_y == from_y - 1) {
		if (to_x != from_x) {
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right"
				<< endl;
			return false;
		}
	}
	else {
		cout
			<< "Error: illegal move - can move only one cell up/down/left/right"
			<< endl;
		return false;
	}

	if (gameBoard.board.at(to_x).at(to_y).getPiece() != 0) {
		if (isPlayer1) {
			if (isupper(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
		else {
			if (islower(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
	}

	return true;
}

