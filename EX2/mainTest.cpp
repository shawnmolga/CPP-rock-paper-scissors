#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RPSGame.h"
using namespace std;

int main(int argc, char *argv[])
{
	RPSGame manager;
	manager.playerAlgoOne = AutoPlayerAlgorithm();
	manager.playerAlgoTwo = AutoPlayerAlgorithm();
	//should be empty
	cout<<"********BEFORE LOCATING PIECES*************"<<endl;
	manager.PrintBoardToConsole();

	int isBad = manager.checkBadFormat();
	if (isBad){
		cout<<"ERROR: auto algorithem has a mistake since positions where illegal!!!"<<endl;
		return 0;
	}
	cout<<"************AFTER POSITION PIECES*******************"<<endl;
	manager.PrintBoardToConsole();

	cout<<"************BEFORE FIRST MOVE*******************"<<endl;
	manager.makeMove();
}

void checkInitValuesOfBoard(){

}
