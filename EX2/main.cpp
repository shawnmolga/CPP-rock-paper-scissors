#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "RPSGame.h"
using namespace std;

int main(int argc, char *argv[])
{
	// check number of args
	if (argc != 2)
	{
		cout
			<< "Usage: Program should get one argument of game method: 'auto-vs-file' / 'auto-vs-auto' / 'file-vs-file' / 'file-vs-auto'"
			<< endl;
		return 0;
	}
	string gameMethod = argv[1];

	unique_ptr<RPSGame> game = make_unique<RPSGame>();
	//check if all files exist and named correctly
	if (!game->initGameMethod(gameMethod))
	{
		return -1;
	}
	int isOK = 0;
	isOK = game->startGame();
	if (isOK == -1 || isOK == -2)
	{
		game->printOutputFile(OUTPUT_FILENAME);
		return -1;
	}

	game->printOutputFile(OUTPUT_FILENAME);
	return 0;
}
