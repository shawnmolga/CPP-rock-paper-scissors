/*
 * RSPPlayer_204157861.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_RSPPlayer_204157861_H_
#define EX2_RSPPlayer_204157861_H_
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
#include <cstdint>
#include <climits>

using namespace std;

class RSPPlayer_204157861 : public PlayerAlgorithm {
private:
	AIBoard gameBoard;
public:
	RSPPlayer_204157861();
	//player algorithm overridden functions
	void PrintBoardToConsole();
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
	virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent�s move
	virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
	virtual unique_ptr<Move> getMove() override;
	virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested

	//position functions and help functions
	void positionUnmovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly);
	void positionPiecesRandomly(int pieceNum, char piece, bool isJoker,char pieceRep, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill);
	void positionFlagsOnBoard(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int& bombsPositioned);
	bool findEmptyNeigbor(int &row, int &col, int flagRow, int flagCol);

	//legality check
	bool isLegalPosition(int row, int col);
	bool checkIsOpponentNeighbors(int row, int col);
	bool isOpponentPiece(char piece);
	bool isLegalMove(unique_ptr<Move> &move, bool isPlayer1);

	//scoring functions
	void calcMovingPieces(const Board &b);
	double calcMaterial(Cell cell);
	double calcDiscovery(AICell cell);
	double calcScore(double material, double discovery, double reveal, int to_x, int to_y);
	void countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces);
	double calcDistanceFromBombOrFlag(int to_x, int to_y);
	int calcDistanceFromPiece(int piece_x, int piece_y, int my_x, int my_y);
	double calcFlagSaftey();
	double calcDistanceFromUnknownPiece(int to_x, int to_y);

	//AI learning functions
	void updateBoard(const Board &b);
	void getBestMove(int &fromRow, int &fromCol, int &toRow, int &toCol);
	double getBestMoveForPiece(double score, const int &fromRow,const int &fromCol, int &toRow, int &toCol);
	double tryMovePiece(unique_ptr<Move>& move);
	void updateProbabilities();
	bool tryToFight(int to_x, int to_y, char myPiece, bool isJoker, bool& isProbOne);
	bool fight(int row, int col, char myPiece, char opponentPiece, bool isMyPieceJoker);
	char shouldChangeJoker(double &score,int joker_x, int joker_y, bool amIPlayerOne);
	int getRandomNumInRange(int start, int end);
	void updateMovingPiece();
	void updateNeighborAsProtectorOrEnemy(char piece, int x, int y, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces);
	void updateOpponentPiece(char piece, bool isOpponentWin, int x, int y, bool isJoker);
	void removeEatenJokersFromCounters();
	void updateIfProbOne(int x, int y);
	void updateNoJokersLeft();
	int getKnownJokersNumOnBoard();
	int getKnownMovingPiecesNumOnBoard();

private:
	int myPlayerNum;
	//opponent piece counters
	int opponentBombsNumOnBoard;
	int opponentFlagsNumOnBoard;
	int opponentPapersNumOnBoard;
	int opponentScissorsNumOnBoard;
	int opponentRocksNumOnBoard;
	int opponentMovingPieceNumOnBoard;
	int opponentJokersNumOnBoard;
	//for future fight
	AICell opponentCell;
	AICell myCell;
	bool willBeFight;
};



#endif /* EX2_RSPPlayer_204157861_H_ */
