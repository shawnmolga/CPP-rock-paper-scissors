#ifndef EX2_POINT_H_
#define EX2_POINT_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Point.h"
#include "RPSBoard.h"
#include "RPSFight.h"
#include "Move.h"
#include "JokerChange.h"
#include "board.h"
#include "RPSpoint.h"
#include "PiecePosition.h"
#include "FilePlayerAlgorithm.h"
#include "player.h"
#endif 

enum GAME_OVER_TYPE
{
	ALL_FLAGS_CAPTURED,
	ALL_PIECES_EATEN,
	WRONG_FILE_FORMAT_ONE,
	WRONG_FILE_FORMAT_TWO,
	WRONG_FILE_FORMAT_BOTH,
	WRONG_MOVE_FILE_FORMAT_ONE,
	WRONG_MOVE_FILE_FORMAT_TWO,
	TIE_NO_WINNER,
	TIE_ALL_FLAGS_EATEN
};
class RPSGame{
	public:
		RPSGame();
		~RPSGame();
        vector<vector<Cell>> board ;  //Noy move the board to be public. we need it in the algorithm
        string ToString(GAME_OVER_TYPE typeGame, int indexErrorPosOne, int indexErrorPosTwo, int indexErrorMoveOne, int indexErrorMoveTwo);
		bool  checkPositionOnBoard(bool &isPlayerOneLegalFormat,bool &isPlayerTwoLegalFormat,std::vector<unique_ptr<FightInfo>>& fights);
        bool isAllGameFilesExists();
        void countNumOfPieces(const int playerNum,int numOfPositionedPieces[],const int piece );
		bool checkPieceOverflow(int numOfPieces[]);
		bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
		bool checkBadFormat();
		void resetGameResults();
		void RPSGame::setGameOver(int winnerNumber, GAME_OVER_TYPE type); 
		//bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker, const std::vector<unique_ptr<FightInfo>>& fights, RPSPoint posOne,RPSPoint posTwo);
		bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker,std::vector<unique_ptr<FightInfo>>& fights,RPSpoint posOne,RPSpoint posTwo);

    private:
        FilePlayerAlgorithm * playerAlgoOne;
        FilePlayerAlgorithm * playerAlgoTwo;

		int indexErrorPosOne; //do we need line numbers?
		int indexErrorPosTwo;
		int indexErrorMoveOne;
		int indexErrorMoveTwo;

		Player playerOne;
		Player playerTwo;
		bool isGameOver;
		GAME_OVER_TYPE gameOverReason;

};