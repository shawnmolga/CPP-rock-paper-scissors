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
    cout << "Error: Missing input files" << endl;
    return 0;
  }
  // string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2,string gameOutputFile
  RCPgameManager mgmtGame1 = new RCPgameManager(argv[1],argv[2],argv[3],argv[4],argv[5]);
  if(!mgmtGame1.checkPositioningInputFiles(argv[1],argv[2])){
    return 0;
  }
  return 0;
}
