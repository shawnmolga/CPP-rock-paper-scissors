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
	string gameMethod; //i.e. auto-vs-auto ..
	Player playerOne;
	Player playerTwo;
	RPSBoard gameBoard;
	GAME_OVER_TYPE gameOverReason;
	int numOfMoves;

public:
	RPSGame();
	~RPSGame();
	string ToString(GAME_OVER_TYPE typeGame);
	void updateJokerMovingPieces();

	//move functions and help-functions
	int handleNonEmptyMove(bool playerOneNonEmpty,std::unique_ptr<Move> & move,bool &isPlayerOneTurn, bool &isGameOverInternal, bool &isBadFormat);
	bool checkDirectionOfMove(int from_x, int from_y, int to_x, int to_y);
	bool checkIfCellTaken(bool isPlayer1, int to_x, int to_y);
	bool isLegalMove(unique_ptr<Move>& move, bool isPlayer1);
	bool movePiece( unique_ptr<Move> &move, bool isPlayerOneTurn, bool &isBadFormat, bool onlyOnePlayes);
	int makeMove();
	int insertToBoard(int playerNum, int x, int y,char inputPiece, bool & isPlayerLegalFormat, bool & isJoker,
			unique_ptr<PiecePosition> & piecePos, RPSFight &fights,
			std::vector<unique_ptr<FightInfo>> &initFights  );

	//game status functions and help-functios
	bool initGameMethod(string gameMethod);
	bool initPlayersAlgo(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
	int startGame();
	int locateOnBoard(int playerNum, std::vector<unique_ptr<PiecePosition>> &vectorToFill, bool &isPlayerLegalFormat, int numOfPositionedPieces[], RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights);
	void printBoardToFile(ofstream & output);
	void printOutputFile(const string &outputFile);
	void updateJokerChange(char prevJokerRep, char newRep, bool isPlayerOne);
	int closeGame(bool & isGameOverInternal, bool & isBadFormat, bool & isPlayerOneTurn, bool isPlayerOneEOF, bool isPlayerTwoEOF);
	int handleEOF(bool playerOneEOF, bool & isBadFormat, bool & isPlayerOneTurn, bool & isGameOverInternal, bool isPlayerOneEOF, bool isPlayerTwoEOF);
	void gameOverSetter(Player* currPlayer, Player* nextPlayer, bool isWinner1, bool isWinner2, GAME_OVER_TYPE type);
	void resetGameResults();
	void setGameOver(int winnerNumber, GAME_OVER_TYPE type);

	//fight functions and help-functions
	void handleSwitchCase(char nextPlayerPiece, RPSFight &fights, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights, RPSFight *ptr);
	void handleNextPlayerPieceFlag(bool isCurrPlayerFlag, char currPlayerPiece, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights,
			RPSFight *ptr, bool isCurrPieceJoker, char currPiece, RPSFight &fights, int nextPlayerNum);
	void handleNexPlayerBomb( char currPlayerPiece, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
			RPSFight &fights, int nextPlayerNum);
	void handlePaperVsRock(Player* currPlayer,
			std::vector<unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
			RPSFight &fights, int nextPlayerNum);
	void handlePaperOrRockVsScissors(int i, Player* nextPlayer, int currPlayerNum,
			std::vector<unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
			RPSFight &fights);
	bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker,RPSFight & fights,std::vector<unique_ptr<FightInfo>> &initFights,RPSpoint posOne,RPSpoint posTwo);

	//legality format checks functions
	bool isAllGameFilesExists(bool isPlayerOneUseFile, bool isPlayerTwoUseFile);
	int checkBadFormat();
	int checkPositionOnBoard(bool &isPlayerOneLegalFormat,bool &isPlayerTwoLegalFormat, RPSFight &fights, std::vector<unique_ptr<FightInfo>>& initFights);
	int checkNumOfPieces(int playerNum, bool isPieceOkPlayer1, bool isPieceOkPlayer2, int inputPiece, bool & isPlayerLegalFormat, int numOfPositionedPieces[]);
	bool checkPieceOverflow(int numOfPieces[]);
	bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
	bool countNumOfPieces(const int playerNum,int numOfPositionedPieces[],const int piece );

	GAME_OVER_TYPE getGameOverReason(){
		return gameOverReason;
	};





};



