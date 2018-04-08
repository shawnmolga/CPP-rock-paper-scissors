#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RCPgameManager.h"
using namespace std;

int main(int argc, char*[]) {
	// check number of args
	if (argc > 1) {
		cout
				<< "Usage: Program should not get any inputs, all input files should be in current folder"
				<< endl;
		return 0;
	}
	// string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2,string gameOutputFile
	RCPgameManager * mgmtGame = new RCPgameManager();

	//check if all files exist and named correctly
	if (!mgmtGame->isAllGameFilesExists()) {
		delete (mgmtGame);
		return -1;
	}

	//check if game already over - bad format or fight result
	//check if file format of possition files is ok and insert possitions to board

	if (mgmtGame->checkBadFormat()) {
		mgmtGame->printOutputFile(OUTPUT_FILENAME);
		//shawn removed - already delete game in pritOutputFile
		//delete (mgmtGame);
		return -1;
	}
	mgmtGame->startGame();
	mgmtGame->printOutputFile(OUTPUT_FILENAME);
	//delete(mgmtGame);
	//return 0;
}
