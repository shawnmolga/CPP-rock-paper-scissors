

#ifndef EX1_CELL_H_
#define EX1_CELL_H_
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
using namespace std;

class Cell
{
	public:
		Cell(char piece , bool isJoker);
		Cell();
		static void cleanCell(Cell& cell);
		char getPiece() const;
		bool getIsJoker() const;
		static bool isPlayerOnePiece(Cell& cell);
		static void updateCell(Cell& cell, char piece, bool isJoker);
		bool checkIsMovingPiece();

		void setIsJoker(bool isJoker_){
			isJoker = isJoker_;
		}
private:
	char piece;
	bool isJoker;
};



#endif /* EX1_CELL_H_ */
