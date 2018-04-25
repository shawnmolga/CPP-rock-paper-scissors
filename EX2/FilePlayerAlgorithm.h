/*
 * FilePlayerAlgorithm.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_FILEPLAYERALGORITHM_H_
#define EX2_FILEPLAYERALGORITHM_H_
#include "PlayerAlgorithm.h"
#include "RPSPiecePosition.h"
#include "RPSpoint.h"
#endif 
class FilePlayerAlgorithm : public PlayerAlgorithm {
public:
	FilePlayerAlgorithm(const string &positionFile, const string &movesFile);
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent�s move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
	virtual int getPieceFromLine(int start, const string &line);
	virtual int getPositionFromLine(int start, const string &line, int &row, int &col);
	virtual bool FilePlayerAlgorithm::checkIfDigit(char c);
	void getPositionAndRepFromLine(const string &line, int playerNum, int &row, int &col,
		char &jokerRep, char &piece);
private:
	bool checkEmptyLine(int start, const string &line);
	string positionFile;
	string movesFile;
};
