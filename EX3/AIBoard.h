#pragma once
#include "Board.h"
#include "Constants.h"
#include <vector>
#include <sstream>
#include "AICell.h"
#include "Point.h"
#include "RPSpoint.h"
class AIBoard : public Board
{
public:
	AIBoard();
	~AIBoard();
	virtual int getPlayer(const Point &pos) const;
	vector<vector<AICell>> board;
	//private:
};
