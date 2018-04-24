/*
 * PlayerAlgorithm.h
 *
 *  Created on: 24 áàôø× 2018
 *      Author: OR
 */

#ifndef EX2_PLAYERALGORITHM_H_
#define EX2_PLAYERALGORITHM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "PiecePosition.h"

using namespace std;

class PlayerAlgorithm {
public:
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill);
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights);
	virtual void notifyOnOpponentMove(const Move& move); // called only on opponent’s move
	virtual void notifyFightResult(const FightInfo& fightInfo); // called only if there was a fight
	virtual unique_ptr<Move> getMove();
	virtual unique_ptr<JokerChange> getJokerChange(); // nullptr if no change is requested

};






#endif /* EX2_PLAYERALGORITHM_H_ */
