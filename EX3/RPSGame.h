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
#include "Player.h"
#include "RSPPlayer_204157861.h"
#include "PlayerAlgorithmInfo.h"


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
	bool isGameOver;
	Player playerOne;
	Player playerTwo;
	unique_ptr <PlayerAlgorithm> playerAlgoOne;
	unique_ptr <PlayerAlgorithm> playerAlgoTwo;
	string gameMethod; //i.e. auto-vs-auto ..
	RPSBoard gameBoard;
	GAME_OVER_TYPE gameOverReason;
	int numOfMoves;
//RPSGame::RPSGame(const unique_ptr <PlayerAlgorithmInfo> & playerOne,const unique_ptr <PlayerAlgorithmInfo> & playerTwo) : isGameOver(false), playerOne(Player(1)), playerTwo(Player(2)), playerAlgoOne(playerOne->getPlayerAlgorithm()), playerAlgoTwo(playerTwo->getPlayerAlgorithm()) {

public:
	RPSGame(const unique_ptr<PlayerAlgorithmInfo>& playerOne, const unique_ptr<PlayerAlgorithmInfo> & playerTwo);
	string ToString(GAME_OVER_TYPE typeGame);
	//void updateJokerMovingPieces();
	void updateJokerPieceRep(int playerNum, char pieceRep);
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
	void PrintBoardToConsole();
	int startGame();
	int locateOnBoard(int playerNum, std::vector<unique_ptr<PiecePosition>> &vectorToFill, bool &isPlayerLegalFormat, int numOfPositionedPieces[], RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights);
	void printBoardToFile(ofstream & output);
	void printOutputFile(const string &outputFile);
	void updateJokerChange(char prevJokerRep, char newRep, bool isPlayerOne);
	int closeGame(bool &isGameOverInternal, bool &isBadFormat, bool &isPlayerOneTurn, bool isPlayerOneEOF,
                       bool isPlayerTwoEOF, bool hasNoMovingPiecePlayerOne, bool hasNoMovingPiecePlayerTwo);	
	int handleEOF(bool playerOneEOF, bool & isBadFormat, bool & isPlayerOneTurn, bool & isGameOverInternal, bool isPlayerOneEOF, bool isPlayerTwoEOF);
	void gameOverSetter(Player* currPlayer, Player* nextPlayer, bool isWinner1, bool isWinner2, GAME_OVER_TYPE type);
	void resetGameResults();
	void setGameOver(int winnerNumber, GAME_OVER_TYPE type);
	void getWinnerInfo(int & winnerNumPlayer,string & gameOverReason);

	//fight functions and help-functions
	void handleSwitchCase(char nextPlayerPiece, RPSFight &fights, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights,unique_ptr <RPSFight> &ptr);
	void handleNextPlayerPieceFlag(bool isCurrPlayerFlag, char currPlayerPiece, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights,
			unique_ptr <RPSFight> &ptr, bool isCurrPieceJoker, char currPiece, RPSFight &fights, int nextPlayerNum);
	void handleNexPlayerBomb( char currPlayerPiece, Player* currPlayer, Player* nextPlayer, int x , int y,
			std::vector<unique_ptr<FightInfo>> &initFights, unique_ptr <RPSFight> &ptr,
			RPSFight &fights, int nextPlayerNum);
	void handlePaperVsRock(Player* currPlayer,
			std::vector<unique_ptr<FightInfo>> &initFights, unique_ptr <RPSFight> &ptr,
			RPSFight &fights, int nextPlayerNum);
	void handlePaperOrRockVsScissors(int i, Player* nextPlayer, int currPlayerNum,
			std::vector<unique_ptr<FightInfo>> &initFights, unique_ptr <RPSFight> &ptr,
			RPSFight &fights);
	bool fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker,RPSFight & fights,std::vector<unique_ptr<FightInfo>> &initFights,RPSpoint posOne,RPSpoint posTwo);

	//legality format checks functions
	int checkBadFormat();
	int checkPositionOnBoard(bool &isPlayerOneLegalFormat,bool &isPlayerTwoLegalFormat, RPSFight &fights, std::vector<unique_ptr<FightInfo>>& initFights);
	int checkNumOfPieces(int playerNum, bool isPieceOkPlayer1, bool isPieceOkPlayer2, int inputPiece, bool & isPlayerLegalFormat, int numOfPositionedPieces[]);
	bool checkPieceOverflow(int numOfPieces[]);
	bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
	bool countNumOfPieces(const int playerNum,int numOfPositionedPieces[],const int piece );
	bool checkMovingPieceLeft(int playerNum);

	GAME_OVER_TYPE getGameOverReason(){
		return gameOverReason;
	};





};


#endif
