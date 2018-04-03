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

  //check if all files exist and 
  if(!mgmtGame.checkPositioningInputFiles(argv[1],argv[2])){
    return -1;
  }
  ifstream posFile1(positionFile1);
  ifstream posFile2(positionFile2);
  mgmtGame.checkInsertPlayerPosition(1,posFile1);
  mgmtGame.checkInsertPlayerPosition(2,posFile2);
  close(posFile1);
  return 0;
}
