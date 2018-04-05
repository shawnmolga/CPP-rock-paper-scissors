#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RCPgameManager.h"
using namespace std;


int main(int argc, char* [])
{
	// check number of args
	if (argc > 1)
	{
		cout << "Usage: Program should not get any inputs, all input files should be in current folder" << endl;
		return 0;
	}
	string positionFile1 = "player1.rps_board";
	string positionFile2 = "player2.rps_board";
	string moveFile1 = "player1.rps_moves";
	string moveFile2 = "player2.rps_moves";
	string outputFile = "rps.output";

	// string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2,string gameOutputFile
	RCPgameManager mgmtGame(positionFile1,positionFile2,moveFile1,moveFile2,outputFile);

	//check if all files exist and named correctly
	if(!mgmtGame.checkInputFiles()){
		return -1;
	}
	//check if game already over - bad format or fight result
	//check if file format of possition files is ok and insert possitions to board
	if (mgmtGame.checkBadFormat() ){
		mgmtGame.printOutputFile(outputFile);
		return -1;
	}

	string output_file = "output.txt";
	mgmtGame.startGame();
	mgmtGame.printOutputFile(output_file);
	return 0;
}
