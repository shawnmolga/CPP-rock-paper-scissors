#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "TournamentManager.h"
using namespace std;

int main(int argc, char *argv[])
{
	TournamentManager& manager = TournamentManager::getTournamentManager();

	if(manager.isValidTournament(argc,argv)){
		cout << "number of players are :" << manager.idToAlgoInfo.size() << endl;
		//manager.startTournament();
		//manager.printTornamentResult();
	}
	// // check number of arg
	// if (argc != 2)
	// {
	// 	cout
	// 		<< "Usage: Program should get one argument of game method: 'auto-vs-file' / 'auto-vs-auto' / 'file-vs-file' / 'file-vs-auto'"
	// 		<< endl;
	// 	return GAME_OVER_SUCC;
	// }
	// string gameMethod = argv[1];

	// unique_ptr<RPSGame> game = make_unique<RPSGame>();
	// //check if all files exist and named correctly
	// if (!game->initGameMethod(gameMethod))
	// {
	// 	return BAD_FORMAT_ERR;
	// }
	// int isOK = GAME_OVER_SUCC;
	// isOK = game->startGame();
	// if (isOK == BAD_FORMAT_ERR || isOK == READ_LINE_POS_ERR)
	// {
	// 	game->printOutputFile(OUTPUT_FILENAME);
	// 	return BAD_FORMAT_ERR; //return -1
	// }

	// game->printOutputFile(OUTPUT_FILENAME);
	// return GAME_OVER_SUCC; //return 0
}
