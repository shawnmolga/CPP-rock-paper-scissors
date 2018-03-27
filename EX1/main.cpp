#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
using namespace std;

char board[ROWS][COLS] = {0};

bool checkPositioningFormat(const string &line, int numOfPieces[], int &row, int &col)
{
  //TODO: ask in forum if we have space or other char in end of input line
  if (line.length < 5)
  {
    cout << "Error: Bad format - line length is smaller than 5 characters" << endl;
    return false;
  }
  bool isJoker = false;
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
  }
  return true;
}

bool checkPieceOverFlow(int numOfPieces[])
{
  if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
  {
    cout << "Error: a piece type appears in file more than its number" << endl;
    return true;
  }
  return false;
}
bool checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile)
{
  //by order of pieces in constans file
  int numOfPieces[6] = {0};
  string line;
  int row;
  int col;
  while (getline(playerPositionFile, line))
  {
    if (!checkPositioningFormat(line, numOfPieces, row, col))
    {
      return false;
    }
    
    if (board[row][col] != 0)
    {
      if(playerNum ==2){
        if(islower(board[row][col])){
            cout << "Error: two or more pieces of player 1 are positioned on the same location" << endl;
            return false;
        }
        else{
          fight();
        }
      }
      else{
        cout << "Error: two or more pieces of player 1 are positioned on the same location" << endl;
        return false;
      }
      
    }
    board[row][col] = line[0];
  }

  if (numOfPieces[5] == 0)
  {
    cout << "Error: flage of player 1 is not positioned on board" << endl;
    return false;
  }
  if (checkPieceOverFlow)
  {
    return false;
  }
}

bool checkPositioningInputFiles(const string &player1PositionFile, const string &player2PositionFile)
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

int main(int argc, char *argv[])
{
  // check number of args
  if (argc < 5)
  {
    cout << "Error: Missing input files" << endl;
    return 0;
  }

  return 0;
}