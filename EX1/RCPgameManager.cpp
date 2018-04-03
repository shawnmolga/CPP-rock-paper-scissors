/*
 * RCPgameManager.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "RCPgameManager.h"

RCPgameManager::RCPgameManager(string PositionFileP1, string PositionFileP2,
                               string moveFilePlayer1, string moveFilePlayer2, string gameOutputFile) : game(RCPgame()), posFileP1(PositionFileP1), posFileP2(PositionFileP2), moveFileP1(moveFilePlayer1), moveFileP2(moveFilePlayer2), outputFile(gameOutputFile)
{
}

RCPgameManager::~RCPgameManager()
{
  delete &game;
}

bool RCPgameManager::checkPositioningFormat(const string &line,
                                            int numOfPieces[], int &row, int &col, bool &isJoker,
                                            char &jokerPiece)
{
  //TODO: ask in forum if we have space or other char in end of input line
  if (line.length() < 5)
  {
    cout << "Error: Bad format - line length is smaller than 5 characters"
         << endl;
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

  if (line[1] != ' ' || line[3] != ' ')
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
    if (line.length() < 7)
    {
      cout
          << "Error: Bad format - line length is smaller than 5 characters"
          << endl;
      return false;
    }
    if (line[5] != ' ')
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
    if (!checkPositioningFormat(line, numOfPieces, row, col, isJoker,
                                piece))
    {
      return false;
    }

    if (game.board[row][col].getPiece() != 0)
    {
      if (playerNum == 2)
      {
        if (islower(game.board[row][col].getPiece()))
        {
          cout
              << "Error: two or more pieces of player 2 are positioned on the same location"
              << endl;
          return false;
        }
        else
        {
          game.fight(row, col, piece, isJoker);
        }
      }
      else
      {
        cout
            << "Error: two or more pieces of player 1 are positioned on the same location"
            << endl;
        return false;
      }
    }
    else
    {
      Cell::updateCell(game.board[row][col], piece, isJoker);
    }
  }

  if (game.getIsGameOver())
  {
  }

  if (numOfPieces[5] == 0)
  {
    cout << "Error: flag of player 1 is not positioned on board" << endl;
    return false;
  }
  if (game.getPlayerOne.checkPieceOverFlow())
  {
    return false;
  }
  if (game.getPlayerTwo.checkPieceOverFlow())
  {
    return true;
  }
}

bool RCPgameManager::checkPositioningInputFiles(const string &player1PositionFile, const string &player2PositionFile)
{
  // Case 1: check if the file exist
  // TODO : check file name - depends how we will get the input files - its not clear!
  if (!player1PositionFile.compare("player1.rps_board") || !player2PositionFile.compare("player2.rps_board"))
  {
    return false;
  }
  ifstream player1File(player1PositionFile);
  ifstream player2File(player2PositionFile);
  if (!player1File.is_open())
  {
    cout << "Error: Player1 position file doesnt exist" << endl;
    player1File.ifstream::close();
    player2File.ifstream::close();
    return false;
  }
  if (!player2File.is_open())
  {
    player1File.ifstream::close();
    player2File.ifstream::close();
    cout << "Error: Player2 position file doesnt exist" << endl;
    return false;
  }
  // Case 2: the files do exists
  player1File.ifstream::close();
  player2File.ifstream::close();
  return true;
}

void RCPgameManager::printOutputFile(string &outputFile)
{
  ofstream output;
  output.open(outputFile, ios::trunc);
  //player 1 is thw winner
  if (game.getPlayerOne().getIsWinner())
  {
    output << "Winner : 1" << endl;
  }
  //player 2 is the winner
  else if (game.getPlayerTwo().getIsWinner())
  {
    output << "Winner : 2" << endl;
  }
  //tie
  else
  {
    output << "Winner : 0" << endl;
  }
  output << endl; // the third line must be en empty line!
  output << game.ToString(game.getGameOverReason()) << endl;
  printBoardToFile(output);
  output.ofstream::close();
}

bool RCPgameManager::checkBadFormat(string &positionFile1, string &positionFile2)
{
  ifstream posFile1(positionFile1);
  ifstream posFile2(positionFile2);
  bool isPlayerOneLegalFormat = checkInsertPlayerPosition(1, posFile1);
  bool isPlayerTwoLegalFormat = checkInsertPlayerPosition(2, posFile2);
  if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat)
  {
    game.setGameOver(0, WRONG_FILE_FORMAT_BOTH);
    posFile1.ifstream::close();
    posFile2.ifstream::close();
    return true;
  }
  else if (!isPlayerOneLegalFormat)
  {
    game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    posFile1.ifstream::close();
    posFile2.ifstream::close();
    return true;
  }

  else if (!isPlayerTwoLegalFormat)
  {
    game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    posFile1.ifstream::close();
    posFile2.ifstream::close();
    return true;
  }
  posFile1.ifstream::close();
  posFile2.ifstream::close();
  return false;
}

void RCPgameManager::printBoardToFile(ofstream &output)
{
  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      if (game.board[i][j].getIsJoker())
      {
        if (Cell::isPlayerOnePiece(game.board[i][j]))
        {
          output << "J";
        }
        else
        {
          output << "j";
        }
      }
      else if (game.board[i][j].getPiece() == 0)
      {
        output << " ";
      }
      else
      {
        output << game.board[i][j].getPiece();
      }
    }
    if (i != ROWS - 1)
    {
      output << endl;
    }
  }
}

void RCPgameManager::Move(const string &player1MoveFile,
                          const string &player2MoveFile)
{

  ifstream player1Move(player1MoveFile);
  ifstream player2Move(player2MoveFile);
  if (!player1Move.is_open())
  {
    if (!player2Move.is_open())
      game.setGameOver(0, WRONG_FILE_FORMAT_BOTH);
  }

  string line1;
  string line2;

  while (getline(player1Move, line1))
  {
    if (makeMove(line1, true))
      break;
    if (getline(player2Move, line2))
    {
      if (makeMove(line2, false))
        break;
    }
    else
      break;
  }

  //one move file is over
  if (player1Move.eof())
  {
    while (getline(player2Move, line2))
    {
      if (makeMove(line2, false))
        break;
    }
  }
  else if (player2Move.eof())
  {
    while (getline(player1Move, line1))
    {
      if (makeMove(line1, true))
        break;
    }
  }

  player1Move.close();
  player2Move.close();
  return;
}

bool RCPgameManager::isLegalMove(int from_x, int from_y, int to_x, int to_y,
                                 bool isPlayer1)
{

  if ((from_x < 1 || from_x > 1 - ROWS) || (to_x < 1 || to_x > 1 - ROWS) || (from_y < 1 || from_y > 1 - COLS) || (to_y < 1 || to_y > 1 - COLS))
  {
    cout << "Error: illegal location on board" << endl;
    return false;
  }

  if (from_x == to_x && from_y == to_y)
  {
    cout << "Error: user MUST move one piece" << endl;
    return false;
  }

  if (game.board[from_x][from_y].getPiece() == BOMB || game.board[from_x][from_y].getPiece() == FLAG)
  {
    cout << "Error: flag/bomb piece is not allowed to move" << endl;
    return false;
  }

  if (to_x == from_x + 1 || to_x == from_x - 1)
  {
    if (to_y != from_y)
    {
      cout << "Error: illegal move " << endl;
      return false;
    }
  }
  else if (to_y == from_y + 1 || to_y == from_y - 1)
  {
    if (to_x != from_x)
    {
      cout << "Error: illegal move" << endl;
      return false;
    }
  }
  else
  {
    cout << "Error: illegal move" << endl;
    return false;
  }

  if (game.board[from_x][from_y].getPiece() != 0)
  {
    if (isupper(game.board[from_x][from_y].getPiece()) == true && isPlayer1 == true)
    {
      if (isupper(game.board[to_x][to_y].getPiece()) != false)
      {
        cout
            << "Error: you are trying to move to a cell taken by your own piece"
            << endl;
        return false;
      }
    }
    else if (isupper(game.board[from_x][from_y].getPiece()) == false && isPlayer1 == false)
    {
      if (isupper(game.board[to_x][to_y].getPiece()) != true)
      {
        cout
            << "Error: you are trying to move to a cell taken by your own piece"
            << endl;
        return false;
      }
    }
    else
      return true;
  }

  return true;
}

bool RCPgameManager::makeMove(string s, bool isPlayer1)
{
  bool isGameOver = false;
  int from_x_int = s[0] - '0';
  int from_y_int = s[2] - '0';
  int to_x_int = s[4] - '0';
  int to_y_int = s[6] - '0';

  if (s[1] != ' ' || s[3] != ' ' || s[5] != ' ' || s[7] != ' ')
  {
    cout << "Error: Bad format - should be space" << endl;
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  //CHECK JOKER REPOSITION
  size_t pos = s.find("[J: ");
  if (pos == string::npos)
  {
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  if (pos != 8)
  {
    cout << "Error: Bad format - Joker information not placed correctly"
         << endl;

    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  size_t posJoker = s.find_first_not_of(" ", pos);

  if (pos == string::npos)
  {
    cout << "Error: Bad format - Joker information not placed correctly"
         << endl;
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  int x_joker = s[posJoker] - '0';
  int y_joker = s[posJoker + 2] - '0';
  if (s[posJoker + 1] != ' ' || s[posJoker + 3] != ' ')
  {
    cout << "Error: Bad format - should be space" << endl;
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  if (game.board[x_joker][y_joker].getIsJoker() == false)
  { //if the original peice is not a joker
    cout << "Error: Piece specified is not joker" << endl;
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  char new_rep = s[8];

  if (new_rep == ROCK || new_rep == PAPER || new_rep == SCISSOR || new_rep == BOMB)
  {
    Cell::updateCell(game.board[x_joker][y_joker], new_rep, true);
  }
  else
  {
    cout << "Error: illegal NEW_REP " << endl;
    isGameOver = true;
    if (isPlayer1)
      game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
    else
      game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
    return isGameOver;
  }

  if (isLegalMove(from_x_int, from_y_int, to_x_int, to_y_int, isPlayer1))
  {
    if (game.board[to_x_int][to_y_int].getPiece() == 0)
    {
      Cell::updateCell(game.board[to_x_int][to_y_int],
                       game.board[from_x_int][from_y_int].getPiece(),
                       game.board[from_x_int][from_y_int].getIsJoker());
      Cell::updateCell(game.board[from_x_int][from_y_int], 0, false);
      isGameOver = game.checkGameOver();
      return isGameOver;
    }
    else
    {
      isGameOver = game.fight(to_x_int, to_y_int,
                              game.board[from_x_int][from_y_int].getPiece(),
                              game.board[from_x_int][from_y_int].getIsJoker());
      return isGameOver;
    }
  }
  isGameOver = true;
  if (isPlayer1)
    game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
  else
    game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
  return isGameOver;
}
