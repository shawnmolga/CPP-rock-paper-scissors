/*
 * RCPgameManager.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "RCPgameManager.h"

RCPgameManager::RCPgameManager(string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2)
: game(new RCPgame()), posFileP1(PositionFileP1), posFileP2(PositionFileP2), moveFileP1(moveFilePlayer1), moveFileP2(moveFilePlayer2){}

RCPgameManager::~RCPgameManager(){
	delete game;
}

bool RCPgameManager::checkPositioningFormat(const string &line, int numOfPieces[], int &row, int &col, bool& isJoker, char& jokerPiece)
{
  //TODO: ask in forum if we have space or other char in end of input line
  if (line.length < 5)
  {
    cout << "Error: Bad format - line length is smaller than 5 characters" << endl;
    return false;
  }

  switch (line[0])
  {
  case ROCK:
    numOfPieces[0]++;
    break;
  case PAPER:
    numOfPieces[1]++;
    break;
  case SCISSOR:
    numOfPieces[2]++;
    break;
  case BOMB:
    numOfPieces[3]++;
    break;
  case JOKER:
    numOfPieces[4]++;
    isJoker = true;
    break;
  case FLAG:
    numOfPieces[5]++;
    break;
  default:
    cout << "Error: Bad format - illegal piece" << endl;
    return false;
  }

  if (line[1] != '\s' || line[3] != '\s')
  {
    cout << "Error: Bad format - should be space" << endl;
    return false;
  }
  row = line[2] - '0';
  col = line[4] - '0';
  if ((row < 1 || row > 1 - ROWS) || (col < 1 || col > 1 - COLS))
  {
    cout << "Error: illegal location on board" << endl;
    return false;
  }
  if (isJoker)
  {
    if (line.length < 7)
    {
      cout << "Error: Bad format - line length is smaller than 5 characters" << endl;
      return false;
    }
    if (line[5] != '\s')
    {
      cout << "Error: Bad format - should be space" << endl;
      return false;
    }
    if (line[6] != ROCK || line[6] != PAPER || line[6] != SCISSOR || line[6] != BOMB)
    {
      cout << "Error: Bad format - illegal piece for joker" << endl;
      return false;
    }
    jokerPiece = line[6];
  }
  return true;
}

bool RCPgameManager::checkPieceOverFlow(int numOfPieces[])
{
  if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
  {
    cout << "Error: a piece type appears in file more than its number" << endl;
    return true;
  }
  return false;
}
bool RCPgameManager::checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile)
{
  //by order of pieces in constans file
  int numOfPieces[6] = {0};
  string line;
  int row;
  int col;
  char piece = line[0];
  bool isJoker = false;
  while (getline(playerPositionFile, line))
  {
	piece = line[0];
	isJoker = false;
    if (!checkPositioningFormat(line, numOfPieces, row, col, isJoker, piece))
    {
      return false;
    }

    if (game.board[row][col].getPiece() != 0)
    {
      if(playerNum ==2){
        if(islower(game.board[row][col].getPiece())){
            cout << "Error: two or more pieces of player 2 are positioned on the same location" << endl;
            return false;
        }
        else{
        	game.fight(row,col,piece,isJoker);
        }
      }
      else{
        cout << "Error: two or more pieces of player 1 are positioned on the same location" << endl;
        return false;
      }

    }
    else{
    	Cell::updateCell(game.board[row][col], piece, isJoker,);
    }

  }

  if (numOfPieces[5] == 0)
  {
    cout << "Error: flag of player 1 is not positioned on board" << endl;
    return false;
  }
  if (checkPieceOverFlow)
  {
    return false;
  }
}

bool RCPgameManager::checkPositioningInputFiles(const string &player1PositionFile, const string &player2PositionFile)
{
  // Case 1: check if the file exist
  // TODO : check file name - depends how we will get the input files - its not clear!
  ifstream player1File(player1PositionFile);
  ifstream player2File(player2PositionFile);
  if (!player1File.is_open())
  {
    cout << "Error: Player1 position file doesnt exist" << endl;
    return false;
  }
  if (!player2File.is_open())
  {
    cout << "Error: Player2 position file doesnt exist" << endl;
    return false;
  }
  // Case 2:

  return true;
}


