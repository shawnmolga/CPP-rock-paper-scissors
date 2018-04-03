#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RCPgameManager.h"
using namespace std;


int main(int argc, char *argv[])
{
  // check number of args
  if (argc < 5)
  {
    cout << "Usage: Program positionFile1 positionFile2 MoveFile1 MoveFile2 OutputFile" << endl;
    return 0;
  }
  string positionFile1 = argv[1];
  string positionFile2 = argv[2];
  string moveFile1 = argv[3];
  string moveFile2 = argv[4];
  string outputFile = argv[5];

  // string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2,string gameOutputFile
  RCPgameManager mgmtGame(positionFile1,positionFile2,moveFile1,moveFile2,outputFile);

  //check if all files exist and named correctly 
  //TODO: figure out if files are in argv[1] or in current directory and change checks 
  if(!mgmtGame.checkPositioningInputFiles(positionFile1,positionFile2)){
    return -1;
  }
  //check if game already over - bad format or fight result
  //check if file format of possition files is ok and insert possitions to board
  if (mgmtGame.checkBadFormat(positionFile1,positionFile2) || mgmtGame.game.checkGameOver()){
    mgmtGame.printOutputFile(outputFile);
    return -1;
  }

  //start game
  mgmtGame.Move(moveFile1,moveFile2);
  mgmtGame.printOutputFile(outputFile);
  return 0;
}
