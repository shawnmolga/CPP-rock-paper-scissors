
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "TournamentManager.h"
using namespace std;

int main(int argc, char *argv[])
{
	TournamentManager & manager = TournamentManager::getTournamentManager();

	if(manager.isValidTournament(argc,argv)){
		cout << "number of players are :" << manager.idToAlgoInfo.size() << endl;
		manager.startTournament();
		manager.printTornamentResult();
	}
}
