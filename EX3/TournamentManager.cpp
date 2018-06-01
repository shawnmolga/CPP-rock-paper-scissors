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
	string playerOneId;
	string playerTwoId;
	while (algorithmsToPlay.size() != 0)
	{
		getPlayersToPlay(playerOneId, playerTwoId);
		if(freeThreadsNum > 0){
			thread t ();
		}
	}
}

void TournamentManager::startNewGame(const string &playerOneId, const string &playerTwoId){
	int winnerNumPlayer;
	string gameOverReason;
	RPSGame game = RPSGame(idToAlgoInfo[playerOneId], idToAlgoInfo[playerTwoId]);
	game.startGame();
	game.getWinnerInfo(winnerNumPlayer,gameOverReason);
	
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
				freeThreadsNum = static_cast<size_t>(strtol(argv[++i], &stringEnd, 10)) - 1;
				if (*stringEnd || freeThreadsNum < 0)
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
		freeThreadsNum = (freeThreadsNum == UNINITIALIZED_ARG) ?  (DEFAULT_THREADS_NUM - 1) : freeThreadsNum;
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
 //TODO: !
}

bool TournamentManager::isValidTournament(int argc, char *argv[])
{
	if(!checkTournamentArguments(argc, argv) || !loadAlgorithms(inputDirPath)){
		return false;
	}
	return true;
}