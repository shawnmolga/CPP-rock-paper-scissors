/*
 * AutoPlayerAlgorithm.h
 *
 *  Created on: 24 áàôø× 2018
 *      Author: OR
 */

#ifndef EX2_AUTOPLAYERALGORITHM_H_
#define EX2_AUTOPLAYERALGORITHM_H_
#include "PlayerAlgorithm.h"

class AutoPlayerAlgorithm : public PlayerAlgorithm {
private:
	RPSBoard gameBoard;
public:
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponentï¿½s move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
};



#endif /* EX2_AUTOPLAYERALGORITHM_H_ */
