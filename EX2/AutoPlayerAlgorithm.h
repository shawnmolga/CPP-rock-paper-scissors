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
#include "RPSpoint.h"
#include "RPSJokerChange.h"
#include "AIBoard.h"
#include "RPSMove.h"
#include <random>

using namespace std;

class AutoPlayerAlgorithm : public PlayerAlgorithm {
private:
	AIBoard gameBoard;
public:
	AutoPlayerAlgorithm();
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent�s move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
	void positionUnmovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly);
	bool isLegalPosition(int row, int col);
	bool checkIsOpponentNeighbors(int player, int row, int col);
	bool findEmptyNeigbor(int &row, int &col, int flagRow, int FlagCol);
	bool isOpponentPiece(int myPlayerNum, char piece);
	void positionPiecesRandomly(int pieceNum, char piece, bool isJoker, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void updateBoard(const Board &b);
	void calcMovingPieces(const Board &b);
	void getBestMove(int &fromRow, int &fromCol, int &toRow, int &toCol);
	double getBestMoveForPiece(double score, const int &fromRow,const int &fromCol, int &toRow, int &toCol);
	double tryMovePiece(unique_ptr<Move>& move,bool isPlayerOneTurn);
	double calcMaterial(Cell cell);
	double calcDiscovery(AICell cell);
	double calcScore(double material, double discovery, double reveal);
	void countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces, bool amIplayerOne);
	double calcDistanceFromBombOrFlag();
	int calcDistanceFromPiece(int piece_x, int piece_y, bool amIPlayerOne);
	void updateFlagProbability();
	bool tryToFight(int to_x, int to_y, char myPiece, bool isJoker);
	bool fight(int row, int col, char myPiece, char opponentPiece, bool isMyPieceJoker);
	char shouldChangeJoker(double &score,int joker_x, int joker_y, bool amIPlayerOne);
	double calcFlagSaftey();
	double calcDistanceFromUnknownPiece();

private:
	int myPlayerNum;
	//opponent piece counters
	int opponentBombsNumOnBoard;
	int opponentMovingPieceNumOnBoard;
	AICell opponentCell;
	AICell myCell;
};



#endif /* EX2_AUTOPLAYERALGORITHM_H_ */
