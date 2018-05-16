#ifndef EX2_POINT_H_
#define EX2_POINT_H_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Point.h"
#include "RPSBoard.h"
#include "RPSFight.h"
#include "RPSMove.h"
#include "RPSJokerChange.h"
#include "RPSBoard.h"
#include "RPSpoint.h"
#include "PiecePosition.h"
#include "FilePlayerAlgorithm.h"
#include "Player.h"
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
        //vector<vector<Cell>> board ;  //Noy move the board to be public. we need it in the algorithm
        string ToString(GAME_OVER_TYPE typeGame, int indexErrorPosOne, int indexErrorPosTwo, int indexErrorMoveOne, int indexErrorMoveTwo);
		int checkPositionOnBoard(bool &isPlayerOneLegalFormat,bool &isPlayerTwoLegalFormat,std::vector<unique_ptr<FightInfo>>& fights);
        bool isAllGameFilesExists(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
        bool initPlayeresAlgo(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
        void countNumOfPieces(const int playerNum,int numOfPositionedPieces[],const int piece );
		bool checkPieceOverflow(int numOfPieces[]);
		bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
		bool checkBadFormat();
		void resetGameResults();
		bool checkBadFormat(bool isPlayerOneLegalFormat, bool isPlayerTwoLegalFormat);
		void setGameOver(int winnerNumber, GAME_OVER_TYPE type);
		//bool isLegalMove(unique_ptr<Move>& move, bool isPlayer1);
		bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker,std::vector<unique_ptr<FightInfo>>& fights,RPSpoint posOne,RPSpoint posTwo);
		GAME_OVER_TYPE getGameOverReason(){
			return gameOverReason;
		};
		int startGame();
		void printBoardToFile(ofstream & output);
		void printOutputFile(const string &outputFile);
		void updateJokerChange(char prevJokerRep, char newRep,
											   bool isPlayerOne);
		bool movePiece(Move& move, JokerChange& playerJokerChange, bool isPlayerOneTurn);
		int makeMove();
		bool initGameMethod(string gameMethod);



    private:
        PlayerAlgorithm * playerAlgoOne;
        PlayerAlgorithm * playerAlgoTwo;

		int indexErrorPosOne; //do we need line numbers?
		int indexErrorPosTwo;
		int indexErrorMoveOne;
		int indexErrorMoveTwo;

		RPSBoard gameBoard;
		Player playerOne;
		Player playerTwo;
		bool isGameOver;
		GAME_OVER_TYPE gameOverReason;
		bool isGameOverInteral;

};
