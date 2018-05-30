/*
 * TournamentManager.h
 *
 *  Created on: 30 במאי 2018
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
#include <list>
#include <random>
#include <mutex>
#include <atomic>

using namespace std;

class TournamentManager {
	static TournamentManager tournamentManager;
	std::map<std::string, unique_ptr<PlayerAlgorithmInfo>(function<unique_ptr<PlayerAlgorithm>()> factoryMethod)> idToAlgoInfo;
	// private ctor
	TournamentManager() {}
	int numOfRegisteredPlayers; //we want to make players play between them in uniformly probability
	list<string> algorithmsToPlay; //set of players that still didnt play 30 games
	list<string> algorithmsPlayed; //algorithms played 30 games

	mutex playersPlayedMutex, idToAlgoInfoMutex;

	void getRandomPlayer(string &playerId, bool doesRestPlayersPlayed);
	int getRandomNumInRange(int start, int end);
	void getPlayersToPlay(string &playerOneId, string &playerTwoId);
	void updatePlayerPlayed(const string& playerOneId, const string& playerTwoId);

public:
	//manager is singleton
	static TournamentManager::TournamentManager& getTournamentManager() {
		return tournamentManager;
	}
	void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);

	~TournamentManager();
	void startTournament();

};

#endif /* EX3_TOURNAMENTMANAGER_H_ */
