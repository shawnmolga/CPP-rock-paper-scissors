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
  if(!RCPgameManager::checkPositioningInputFiles(argv[1],argv[2])){
    return 0;
  }

  return 0;
}
