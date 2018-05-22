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
#include <cstdint> //or <stdint.h>
#include <climits>

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
	bool checkIsOpponentNeighbors(int row, int col);
	bool findEmptyNeigbor(int &row, int &col, int flagRow, int flagCol);
	bool isOpponentPiece(char piece);
	void positionPiecesRandomly(int pieceNum, char piece, bool isJoker,char pieceRep, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void updateBoard(const Board &b);
	void calcMovingPieces(const Board &b);
	void getBestMove(int &fromRow, int &fromCol, int &toRow, int &toCol);
	double getBestMoveForPiece(double score, const int &fromRow,const int &fromCol, int &toRow, int &toCol);
	double tryMovePiece(unique_ptr<Move>& move);
	double calcMaterial(Cell cell);
	double calcDiscovery(AICell cell);
	double calcScore(double material, double discovery, double reveal, int to_x, int to_y);
	void countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces);
	double calcDistanceFromBombOrFlag(int to_x, int to_y);
	int calcDistanceFromPiece(int piece_x, int piece_y, int my_x, int my_y);
	void updateFlagProbability();
	bool tryToFight(int to_x, int to_y, char myPiece, bool isJoker, bool& isProbOne);
	bool fight(int row, int col, char myPiece, char opponentPiece, bool isMyPieceJoker);
	char shouldChangeJoker(double &score,int joker_x, int joker_y, bool amIPlayerOne);
	double calcFlagSaftey();
	double calcDistanceFromUnknownPiece(int to_x, int to_y);
	int getRandomNumInRange(int start, int end);
	void updateMovingPiece();
	bool isLegalMove(unique_ptr<Move> &move, bool isPlayer1);
	void printVectorPositions(std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void PrintBoardToConsole();
	void positionFlagsOnBoard(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int& bombsPositioned);
	void updateNeighborAsProtectorOrEnemy(char piece, int x, int y, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces);
	
private:
	int myPlayerNum;
	//opponent piece counters
	int opponentBombsNumOnBoard;
	int opponentFlagsNumOnBoard;
	int opponentMovingPieceNumOnBoard;
	AICell opponentCell;
	AICell myCell;
	bool willBeFight;
};



#endif /* EX2_AUTOPLAYERALGORITHM_H_ */
