/*
 * TournamentManager.cpp
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#include "TournamentManager.h"

TournamentManager TournamentManager::tournamentManager;

TournamentManager::~TournamentManager()
{
	numOfRegisteredPlayers = 0; //not multi threaded since there is only one thread in this point - registration point
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod)
{
	numOfRegisteredPlayers++;
	if (idToAlgoInfo.find(id) == idToAlgoInfo.end())
	{ //doesnt exist
		algorithmsToPlay.push_back(id);

		idToAlgoInfo[id] = make_unique<PlayerAlgorithmInfo>(factoryMethod);
	}
	else
	{
		cout << "Error in registration: this id already registered. ID:" << id << endl;
	}
}

void TournamentManager::startTournament()
{
	ThreadPool pool(numOfThreads);
	string playerOneId;
	string playerTwoId;
	while (algorithmsToPlay.size() != 0)
	{
		getPlayersToPlay(playerOneId, playerTwoId);
		//bind parameters to function
		//if the if wont change we need to do : ref(playerOneId), ref(playerTwoID)
		pool.doJob (std::bind(startNewGame,playerOneId, playerTwoId)); //adds this game to "pool of jobs", when a thread is done the thread will do this job
	}
}

void  TournamentManager::updateScore(RPSGame & game,const string &playerOneId, const string &playerTwoId){
	int winnerNumPlayer;
	string gameOverReason; // TODO: do we need this?
	game.getWinnerInfo(winnerNumPlayer,gameOverReason);
	unique_lock<mutex> lock(updateScoreMutex);
	if(winnerNumPlayer == 1){
		idToAlgoInfo[playerOneId]->score = idToAlgoInfo[playerOneId]->score + 3;
	}
	else if(winnerNumPlayer == 2){
		idToAlgoInfo[playerTwoId]->score = idToAlgoInfo[playerTwoId]->score + 3;
	}
	else {
		idToAlgoInfo[playerOneId]->score ++;	
		idToAlgoInfo[playerTwoId]->score ++;
	}
	lock.unlock();
}

void TournamentManager::startNewGame(const string &playerOneId, const string &playerTwoId){
	int winnerNumPlayer;
	string gameOverReason;
	RPSGame game = RPSGame(idToAlgoInfo[playerOneId], idToAlgoInfo[playerTwoId]);
	game.startGame();
	updateScore(game, playerOneId,playerTwoId);
}

void TournamentManager::printTornamentResult(){
//iterating over the map and print the results 
  for (std::map<std::string, unique_ptr<PlayerAlgorithmInfo>>::iterator it=idToAlgoInfo.begin(); it!=idToAlgoInfo.end(); ++it)
    std::cout << it->first << " " << it->second->score << '\n';
}

void TournamentManager::getPlayersToPlay(string &playerOneId, string &playerTwoId)
{

	if (algorithmsToPlay.size() == 1)
	{
		playerOneId = algorithmsToPlay[0];
		getRandomPlayer(playerTwoId, true);
	}
	else
	{
		getRandomPlayer(playerOneId, false);
		algorithmsToPlay.erase(find(algorithmsToPlay.begin(),algorithmsToPlay.end(),playerOneId));
		getRandomPlayer(playerTwoId, false);
		algorithmsToPlay.push_back(playerOneId);
	}
	updatePlayerPlayed(playerOneId, playerTwoId);
}

void TournamentManager::updatePlayerPlayed(const string &playerOneId, const string &playerTwoId)
{

	if (idToAlgoInfo[playerOneId]->gamesPlayed == 30)
	{
		unique_lock<mutex> lock(playersPlayedMutex);
		algorithmsPlayed.push_back(playerOneId);
		algorithmsToPlay.erase(find(algorithmsToPlay.begin(),algorithmsToPlay.end(),playerOneId));
		lock.unlock();
	}

	if (idToAlgoInfo[playerTwoId]->gamesPlayed == 30)
	{
		unique_lock<mutex> lock(playersPlayedMutex);
		algorithmsPlayed.push_back(playerTwoId);
		algorithmsToPlay.erase(find(algorithmsToPlay.begin(),algorithmsToPlay.end(),playerTwoId));
		lock.unlock();
	}
}

void TournamentManager::getRandomPlayer(string &playerId, bool doesRestPlayersPlayed)
{
	vector<string> &algoList = doesRestPlayersPlayed ? algorithmsPlayed : algorithmsToPlay;
	int index = getRandomNumInRange(0, (algoList.size() - 1));
	playerId = algoList[index];
}

int TournamentManager::getRandomNumInRange(int start, int end)
{
	std::random_device rd;								// obtain a random number from hardware
	std::mt19937 eng(rd());								// seed the generator
	std::uniform_int_distribution<> yDistr(start, end); // define the range
	return yDistr(eng);
}

bool TournamentManager::checkTournamentArguments(int argc, char *argv[])
{
	std::string path = ".";

	if (argc > 5)
	{
		cout
			<< "Usage: Too Many Arguments, optional arguments are: -threads <num_thread> -path <location_of_algorithm>"
			<< endl;
		return false;
	}

	for (auto i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-threads") == 0)
		{
			if (i == argc - 1)
			{ /* we got threads flag but this is the last argument - this is an error because we need the int*/
				std::cout << "Error: got -threads flag, but threads value is missing." << std::endl;
				return false;
			}
			else
			{
				char *stringEnd = nullptr;
				//strtoll convert string to int
				numOfThreads = static_cast<size_t>(strtol(argv[++i], &stringEnd, 10)) - 1;
				if (*stringEnd || numOfThreads < 0)
				{
					std::cout << "Error: -threads flag value is not a valid positive integer." << std::endl;
					return false;
				}
			}
		}
		else if (strcmp(argv[i], "-path") == 0)
		{
			//path = argv[++i]; /* this is the dir path */
			if (i == argc - 1)
			{ /* we got path flag but this is the last argument - this is an error because we need the path*/
				std::cout << "Error: got -path flag, but path value is missing." << std::endl;
				return false;
			}
			else
			{
				path = argv[++i];
			}
		}
		else
		{
				std::cout << "Error: Unrecognize flag optional arguments are: -threads <num_thread> -path <location_of_algorithm" << std::endl;
				return false;
		}
	}
		if (!isValidDir(path))
		{ /* checks if directory in dir_path exists */
			std::cout << "Wrong path: " << path << std::endl;
			return false;
		}
		inputDirPath = path;
		numOfThreads = (numOfThreads == UNINITIALIZED_ARG) ?  (DEFAULT_THREADS_NUM - 1) : numOfThreads;
		return true;
}

bool TournamentManager::isValidDir(const string & path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(".")) != NULL)
	{
		closedir(dir);
		return true;
	}
	else
	{
		std::cout
			<< "Unable to open temp dirList file in working directory, Exit from Game."
			<< std::endl;
		return false;
	}
	return false;
}

bool TournamentManager::loadAlgorithms(const string & path)
{
	FILE *dl;   // handle to read directory 
    const char *command_str = "ls *.so";  // command string to get dynamic lib names
    dl = popen(command_str, "r"); 

}

bool TournamentManager::isValidTournament(int argc, char *argv[])
{
	if(!checkTournamentArguments(argc, argv) || !loadAlgorithms(inputDirPath)){
		return false;
	}
	return true;
}

bool TournamentManager::loadAlgorithemsFromPath() {
	FILE *dl;   // handle to read directory
	const char *command_str = "ls *.so";  // command string to get dynamic lib names
	char in_buf[BUF_SIZE]; // input buffer for lib names
	list<void *> dl_list; // list to hold handles for dynamic libs
	list<void *>::iterator itr;
	std::map<std::string, unique_ptr<PlayerAlgorithmInfo>>::iterator fitr;
	// get the names of all the dynamic libs (.so  files) in the current dir
	dl = popen(command_str, "r");
	if(!dl) {
		cout << "Error: failed to open .so files" << endl;
		return false;
	}
	void *dlib;
	char name[BUF_SIZE];
	while(fgets(in_buf, BUF_SIZE, dl)){
		// trim off the whitespace
		char *ws = strpbrk(in_buf, " \t\n");
		if(ws) *ws = '\0';
		// append ./ to the front of the lib name
		sprintf(name, "./%s", in_buf);
		dlib = dlopen(name, RTLD_NOW);
		if(dlib == NULL){
			cout << "Error: algorithm failed to open." << endl;
			return false;
		}
		// add the handle to our list
		dl_list.insert(dl_list.end(), dlib);
	}
}
