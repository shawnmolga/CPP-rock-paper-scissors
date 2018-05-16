#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Cell.h"
using namespace std;

class AICell : public Cell {
	AICell(char piece, bool isJoker);
	AICell();
	bool isJokerKnown;
	bool isMovingPieceKnown;
	bool isMovingPiece;
	int	flagProbability;
	void initAIFields();
	void updateCellKnowlage(AICell cell, AICell fromCell);
};