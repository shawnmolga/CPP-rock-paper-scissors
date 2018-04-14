#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RPSgameManager.h"
using namespace std;

int main(int argc, char*[]) {
	// check number of args
	if (argc > 1) {
		cout
				<< "Usage: Program should not get any inputs, all input files should be in current folder"
				<< endl;
		return 0;
	}
	RPSgameManager * mgmtGame = new RPSgameManager();
	//check if all files exist and named correctly
	if (!mgmtGame->isAllGameFilesExists()) {
		delete (mgmtGame);
		return -1;
	}

	//check if game already over - bad format or fight result
	//check if file format of possition files is ok and insert possitions to board

	if (mgmtGame->checkBadFormat()) {
		mgmtGame->printOutputFile(OUTPUT_FILENAME);
		delete (mgmtGame);
		return -1;
	}
	int isOK = 0;
	isOK = mgmtGame->startGame();
	if (isOK == -1){
		cout << "startGame returned -1" << endl;
		delete(mgmtGame);
		return 1;
	}

	mgmtGame->printOutputFile(OUTPUT_FILENAME);
	delete(mgmtGame);
	return 0;
}
