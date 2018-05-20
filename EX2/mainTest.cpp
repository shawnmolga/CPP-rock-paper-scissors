#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RPSGame.h"
using namespace std;

int main()
{
	RPSGame manager;
	manager.initPlayersAlgo(false,false);
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

	bool isPlayerOneTurn = true;
	bool isAboutToMove = true;
	cout << "beforecheckGameOver" << endl;
	if (manager.checkGameOver(isAboutToMove, isPlayerOneTurn))
	{
		cout<<"GAME OVER BEFORE MOVE............"<<endl;
		return 0;
	}

	cout<<"************BEFORE FIRST MOVE*******************"<<endl;
	manager.makeMove();
}

void checkInitValuesOfBoard(){

}
