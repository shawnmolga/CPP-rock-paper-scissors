/*
 * AutoPlayerAlgorithm.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_AUTOPLAYERALGORITHM_H_
#define EX2_AUTOPLAYERALGORITHM_H_
#include "PlayerAlgorithm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RPSPiecePosition.h"
#include <random>

using namespace std;

class AutoPlayerAlgorithm : public PlayerAlgorithm {
private:
	RPSBoard gameBoard;
public:
	AutoPlayerAlgorithm();
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent�s move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
	void positionFlags(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	bool isLegalPosition(int row, int col);
	bool checkIsOpponentNeighbors(int player, int row, int col);
};



#endif /* EX2_AUTOPLAYERALGORITHM_H_ */
