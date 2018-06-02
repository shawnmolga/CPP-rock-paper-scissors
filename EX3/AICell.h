#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Cell.h"
using namespace std;

class AICell : public Cell {
	public:
	AICell(char piece, bool isJoker);
	AICell();
	bool isJokerKnown;
	bool isMovingPieceKnown;
	bool isMovingPiece;
	double flagProbability;
	double paperProbability;
	double scissorsProbability;
	double rockProbability;
	double jokerProbability;
	//int x;
	//int y;
	//void setX(int x);
	//void setY(int y);
	void initAIFields();
	static void updateCellKnowlage(AICell & cell, const AICell & fromCell);
	bool isMyPiece(int myPlayerNum);
};
