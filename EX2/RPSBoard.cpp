/*
 * RPSBoard.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSBoard.h"

RPSBoard::RPSBoard(){
	vector <vector<Cell>> board(ROWS, vector<Cell>(COLS));
	cout << "in RPSBoard()" << endl;
		for(int i=0;i<ROWS;i++) {
			vector <Cell> rowVector;
			for(int j=0;j<COLS;j++){
				rowVector.push_back(Cell(0,false));
			}
			board.push_back(rowVector);
		}
	//board = vector(ROWS, vector<Cell>(COLS)); //	board = new Cell *[ROWS];
}
RPSBoard::~RPSBoard(){}

int RPSBoard::getPlayer(const Point& pos) const{
	int x = pos.getX();
	int y = pos.getY();
	char piece = board.at(x-1).at(y-1).getPiece();
	if (piece == 0) return 0;
	else if (isupper(piece)) return 1;
	else return 2;
}

