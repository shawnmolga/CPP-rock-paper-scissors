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
#include "RPSGame.h"

using namespace std;

class TournamentManager {
	static TournamentManager tournamentManager;
	static const int UNINITIALIZED_ARG = -1;
	static const size_t DEFAULT_THREADS_NUM = 4;
	std::map<std::string, unique_ptr<PlayerAlgorithmInfo>> idToAlgoInfo;
	// private ctor
	TournamentManager() {}
	int numOfRegisteredPlayers; //we want to make players play between them in uniformly probability
	vector<string> algorithmsToPlay; //set of players that still didnt play 30 games
	vector<string> algorithmsPlayed; //algorithms played 30 games

	mutex playersPlayedMutex, idToAlgoInfoMutex, updateScoreMutex;
	void  updateScore(RPSGame & game,const string &playerOneId, const string &playerTwoId);
	void getRandomPlayer(string &playerId, bool doesRestPlayersPlayed);
	int getRandomNumInRange(int start, int end);
	void getPlayersToPlay(string &playerOneId, string &playerTwoId);
	void updatePlayerPlayed(const string& playerOneId, const string& playerTwoId);
	size_t freeThreadsNum;
	bool isValidDir(const string & path);
	string inputDirPath;
	bool loadAlgorithms(const string & path);
    void startNewGame(const string &playerOneId, const string &playerTwoId);
public:
	//manager is singleton
	static TournamentManager::TournamentManager& getTournamentManager() {
		return tournamentManager;
	}
	void printTornamentResult();
	bool checkTournamentArguments(int argc, char * argv[]);
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
	bool isValidTournament(int argc, char *argv[]);
	~TournamentManager();
	void startTournament();

};

#endif /* EX3_TOURNAMENTMANAGER_H_ */
