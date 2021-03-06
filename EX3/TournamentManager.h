/*
 * TournamentManager.h
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#ifndef EX3_TOURNAMENTMANAGER_H_
#define EX3_TOURNAMENTMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include "PlayerAlgorithm.h"
#include "PlayerAlgorithmInfo.h"
#include "Constants.h"
#include <list>
#include <random>
#include <mutex>
#include "dirent.h"
#include <atomic>
#include <vector>
#include <thread>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "RPSGame.h"
#include <set>

using namespace std;

class TournamentManager {
	static TournamentManager tournamentManagerSingelton;
	static const int counterGet = 0;
	static const int UNINITIALIZED_ARG = -1;
	static const size_t DEFAULT_THREADS_NUM = 4;

	int numOfThreads = DEFAULT_THREADS_NUM ;// if num_of_threads entered, reassign ThreadsNum in checkTournamentArguments
	//ThreadPool pool; //initialized in startTournament()
    std::vector<std::thread> threadPool_vector;

    void threadEntry();

        // size of buffer for reading in directory entries
	static const unsigned int BUF_SIZE = 4096;

	// private ctor
	TournamentManager();
	int numOfRegisteredPlayers; //we want to make players play between them in uniformly probability
	vector<string> algorithmsToPlay; //set of players that still didnt play 30 games
	vector<string> algorithmsPlayed; //algorithms played 30 games
	list<void *> dl_list; // list to hold handles for dynamic libs
	list<void *>::iterator itr;
	mutex playersPlayedMutex, idToAlgoInfoMutex, updateScoreMutex, algorithmsToPlayMutex;
	void  updateScore(RPSGame & game,const string &playerOneId, const string &playerTwoId);
	void getRandomPlayer(string &playerId, bool doesRestPlayersPlayed);
	int getRandomNumInRange(int start, int end);
	void getPlayersToPlay(string &playerOneId, string &playerTwoId);
	void updatePlayerPlayed(const string& playerOneId, const string& playerTwoId);
	bool isValidDir(const string & path);
	string inputDirPath;
    void startNewGame(const string &playerOneId, const string &playerTwoId);
    bool loadAlgorithemsFromPath();
    void closeAlgorithemLibs();
 	TournamentManager(const TournamentManager&) = delete;
	TournamentManager& operator=(const TournamentManager&)	= delete;
    public:
	//manager is singleton
	static TournamentManager & getTournamentManager(){
		return tournamentManagerSingelton;
	}
	void copyString(string & src, string toCopy);
	void printTornamentResult();
	bool checkTournamentArguments(int argc, char * argv[]);
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
	bool isValidTournament(int argc, char *argv[]);
	~TournamentManager();
	void startTournament();
	std::map<std::string, unique_ptr<PlayerAlgorithmInfo>> idToAlgoInfo;

};

#endif /* EX3_TOURNAMENTMANAGER_H_ */
