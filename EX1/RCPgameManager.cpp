/*
 * RCPgameManager.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "RCPgameManager.h"

RCPgameManager::RCPgameManager(string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2,string gameOutputFile)
: game(RCPgame()), posFileP1(PositionFileP1), posFileP2(PositionFileP2), moveFileP1(moveFilePlayer1), moveFileP2(moveFilePlayer2), outputFile(gameOutputFile){}

RCPgameManager::~RCPgameManager()
{
  delete & game;
}

bool RCPgameManager::checkPositioningFormat(const string &line, int numOfPieces[], int &row, int &col, bool &isJoker, char &jokerPiece)
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
bool RCPgameManager::checkInsertPlayerPosition(int playerNum, ifstream &playerPositionFile)
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
      if (playerNum == 2)
      {
        if (islower(game.board[row][col].getPiece()))
        {
          cout << "Error: two or more pieces of player 2 are positioned on the same location" << endl;
          return false;
        }
        else
        {
          game.fight(row, col, piece, isJoker);
        }
      }
      else
      {
        cout << "Error: two or more pieces of player 1 are positioned on the same location" << endl;
        return false;
      }
    }
    else
    {
      Cell::updateCell(game.board[row][col], piece, isJoker);
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
  if(!player1PositionFile.compare("player1.rps_board") ||!player2PositionFile.compare("player2.rps_board")) {
    return false;
  }
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
  // Case 2: the files do exists
  return true;
}

void RCPgameManager::printOutputFile(string &outputFile)
{
  ofstream output("outputFile");
  //player 1 is thw winner
  if(game.getPlayerOne.isWinner())
  {
    output << "Winner : 1" <<endl;
  }
  //player 2 is the winner
  else if(game.getPlayerTwo.isWinner()){
    output << "Winner : 2" <<endl;
  }
  //tie
  else {
    output << "Winner : 0" <<endl;
  }
  output <<game.ToString(game.getGameOverReason())<< endl;
}
void printBoardToFile(ofstream &outFile)
{

}

bool RCPgameManager::Move(const string &player1MoveFile, const string &player2MoveFile)
{
  //checks if player moved peice (must)
  //cheks if player changes one Joker from current representation to another (optional)

  //check if one piece moved
  ifstream player1Move(player1MoveFile);
  ifstream player2Move(player2MoveFile);
  if (!player1Move.is_open())
  {
    cout << "Error: Player1 move file doesnt exist" << endl;
    return false;
  }
  if (!player2Move.is_open())
  {
    cout << "Error: Player2 move file doesnt exist" << endl;
    return false;
  }
  //loop:
  //get line from player1 file and make move
  //get line from player2 file and make move
  //if all ok, continue loop
  string line1;
  string line2;
  while (getline(player1Move, line1))
  {
    if (!makeMove(line1, true))
    {
      //player1 loses round;
      if (getline(player2Move, line2))
      {
        if (!makeMove(line2, false))
        {
          //player2 loses round
        }
      }
      else if (player2Move.eof())
      {
        continue;
      }
      else
      {
        cout << "Error: Player2 move file error" << endl; //todo
        continue;                                         //todo check this
      }
    }
    else
    {
      //player1 wins round
      if (getline(player2Move, line2))
      {
        if (!makeMove(line2, false))
        {
          //player2 loses round
        }
      }
      else if (player2Move.eof())
      {
        continue;
      }
      else
      {
        cout << "Error: Player2 move file error" << endl; //todo
        continue;                                         //todo check this
      }
    }
  } //end of while loop - player1 file move over
    //player2Move file not done
    //continue executing player2Move until game is over.
  if (player1Move.eof())
  {
    while (getline(player2Move, line2))
    {
      makeMove(line2, false);
    }
  }
  else if (player2Move.eof())
  {
    while (getline(player1Move, line1))
    {
      makeMove(line1, true);
    }
  }
  else
  {
    //end game - what happens?
  }

  //end of game
}

bool RCPgameManager::isLegalMove(int from_x, int from_y, int to_x, int to_y, bool isPlayer1)
{
  if (isupper(game.board[from_x][from_y].getPiece()) == true && isPlayer1 == true)
  {
    if (isupper(game.board[to_x][to_y].getPiece()) == false)
    {
      return true;
    }
  }
  if (isupper(game.board[from_x][from_y].getPiece()) == false && isPlayer1 == false)
  {
    if (isupper(game.board[to_x][to_y].getPiece()) == true)
    {
      return true;
    }
  }
  return false;
}

bool RCPgameManager::makeMove(string s, bool isPlayer1)
{
  bool isWinner = false;
  int from_x_int = s[0] - '0';
  int from_y_int = s[2] - '0';
  int to_x_int = s[4] - '0';
  int to_y_int = s[6] - '0';
  if ((from_x_int < 1 || from_x_int > 1 - ROWS) || (to_x_int < 1 || to_x_int > 1 - ROWS) || (from_y_int < 1 || from_y_int > 1 - COLS) || (to_y_int < 1 || to_y_int > 1 - COLS)) {
    cout << "Error: illegal location on board" << endl;
    isWinner = false;
    return isWinner;
	}
	if (from_x_int == to_x_int && from_y_int == to_y_int) {
    cout << "Error: user MUST move one piece" << endl;
    isWinner = false;
    return isWinner;
	}
	if (s[1] != '/s' || s[3] != '/s' || s[5] != '/s' || s[7] != '/s') {
    cout << "Error: Bad format - should be space" << endl;
    isWinner = false;
    return isWinner;
	}

	// check : if there is a player1 peice in (to_x_int, to_y_int) return false
	if (isLegalMove(from_x_int, from_y_int, to_x_int, to_y_int,isPlayer1)) { //ASK!
		game.fight(from_x_int, from_y_int, game.board[to_x_int][to_y_int].getPiece(), game.board[to_x_int][to_y_int].getIsJoker() );
    }
    else
    {
      isWinner = false;
      return isWinner;
    }

    //CHECK JOKER REPOSITION
    size_t pos = s.find("[J: ");
    if (pos == string::npos)
    {
      return isWinner;
    }
    if (pos != 8)
    {
      cout << "Error: Bad format - Joker information not placed correctly" << endl;
      isWinner = false;
      return isWinner;
    }
    size_t posJoker = s.find_first_not_of(" ", pos);
    if (pos == string::npos)
    {
      cout << "Error: Bad format - Joker information not placed correctly" << endl;
      isWinner = false;
      return isWinner;
    }
    int x_joker = s[posJoker] - '0';
    int y_joker = s[posJoker + 2] - '0';
    if (s[posJoker + 1] != '/s' || s[posJoker + 3] != '/s')
    {
      cout << "Error: Bad format - should be space" << endl;
      isWinner = false;
      return isWinner;
    }
    if (game.board[x_joker][y_joker].getIsJoker() == false)
    { //if the original peice is not a joker
      cout << "Error: Piece specified is not joker" << endl;
      isWinner = false;
      return isWinner;
    }

    char new_rep = s[8];
    
    if (game.board[x_joker][y_joker].getPiece()==(new_rep))
    {
      //joker representation did not change
      return isWinner;
    }
    if (new_rep == ROCK || new_rep == PAPER || new_rep == SCISSOR || new_rep == BOMB )
    {
      game.board[x_joker][y_joker].updateCell(game.board[x_joker][y_joker], new_rep, true);
      return isWinner;
    }
    else
      cout << "Error: illegal NEW_REP " << endl;
    isWinner = false;
  //return ??
}