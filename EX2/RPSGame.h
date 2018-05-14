#ifndef EX2_POINT_H_
#define EX2_POINT_H_
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
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
#include "AutoPlayerAlgorithm.h"
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
	TIE_ALL_FLAGS_EATEN,
	TOO_MANY_MOVES
};


class RPSGame{
private:
	PlayerAlgorithm * playerAlgoOne;
	PlayerAlgorithm * playerAlgoTwo;
	bool isGameOver;
	string gameMethod;

	Player playerOne;
	Player playerTwo;

	RPSBoard gameBoard;
	GAME_OVER_TYPE gameOverReason;
	int numOfMoves;

public:
		RPSGame();
		~RPSGame();
        //vector<vector<Cell>> board ;  //Noy move the board to be public. we need it in the algorithm
        string ToString(GAME_OVER_TYPE typeGame);
		int checkPositionOnBoard(bool &isPlayerOneLegalFormat,bool &isPlayerTwoLegalFormat, RPSFight &fights, std::vector<unique_ptr<FightInfo>>& initFights);
        bool isAllGameFilesExists(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
        bool initPlayersAlgo(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
        bool countNumOfPieces(const int playerNum,int numOfPositionedPieces[],const int piece );
		bool checkPieceOverflow(int numOfPieces[]);
		bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
		int checkBadFormat();
		void resetGameResults();
		void setGameOver(int winnerNumber, GAME_OVER_TYPE type);
		bool isLegalMove(unique_ptr<Move>& move, bool isPlayer1);
		bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker,RPSFight & fights,std::vector<unique_ptr<FightInfo>> &initFights,RPSpoint posOne,RPSpoint posTwo);
		GAME_OVER_TYPE getGameOverReason(){
			return gameOverReason;
		};
		int startGame();
		int locateOnBoard(int playerNum, std::vector<unique_ptr<PiecePosition>> &vectorToFill, bool &isPlayerLegalFormat, int numOfPositionedPieces[], RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights);
		void printBoardToFile(ofstream & output);
		void printOutputFile(const string &outputFile);
		void updateJokerChange(char prevJokerRep, char newRep,
											   bool isPlayerOne);

							
		bool movePiece( unique_ptr<Move> &move, unique_ptr<JokerChange> &playerJokerChange, bool isPlayerOneTurn);
		int makeMove();
		bool initGameMethod(string gameMethod);
		void PrintBoardToConsole();
};
