/*
 * TournamentManager.cpp
 *
 *  Created on: 30 במאי 2018
 *      Author: OR
 */

#include "TournamentManager.h"

TournamentManager TournamentManager::tournamentManager;

TournamentManager::~TournamentManager() {
	numOfRegisteredPlayers = 0; //not multi threaded since there is only one thread in this point - registration point
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
	numOfRegisteredPlayers++;
	if (idToAlgoInfo.find(id) == idToAlgoInfo.end()) { //doesnt exist
		algorithmsToPlay.push_back(id);
		idToAlgoInfo[id] = make_unique<PlayerAlgorithmInfo>(factoryMethod);
	}
	else {
		cout << "Error in registration: this id already registered. ID:" << id << endl;
	}
}

void TournamentManager::startTournament() {
	string playerOneId;
	string playerTwoId;
	while (algorithmsToPlay.size() != 0) {
		getPlayersToPlay(playerOneId, playerTwoId);
	}
}

void TournamentManager::getPlayersToPlay(string &playerOneId, string &playerTwoId) {

	if (algorithmsToPlay.size() == 1) {
		playerOneId = algorithmsToPlay[0];
		getRandomPlayer(playerTwoId, true);
	}
	else {
		getRandomPlayer(playerOneId, false);
		algorithmsToPlay.remove(playerOneId);
		getRandomPlayer(playerTwoId, false);
		algorithmsToPlay.push_back(playerOneId);
	}

	updatePlayersPlayed(playerOneId, plaerTwoId);

}

void TournamentManager::updatePlayerPlayed(const string& playerOneId, const string& playerTwoId) {

	if (idToAlgoInfo[playerOneId]->gamesPlayed == 30) {
		unique_lock<mutex> lock(playerPlayedMutex);
		algorithmsPlayed.push_back(playerOneId);
		algorithmsToPlay.remove(playerOneId);
		lock.unlock();
	}

	if (idToAlgoInfo[playerTwoId]->gamesPlayed == 30) {
		unique_lock<mutex> lock(playerPlayedMutex);
		algorithmsPlayed.push_back(playerTwoId);
		algorithmsToPlay.remove(playerTwoId);
		lock.unlock();
	}
}

void TournamentManager::getRandomPlayer(string &playerId, bool doesRestPlayersPlayed) {
	vector<string>& algoList = doesRestPlayersPlayed ? algorithmsPlayed : algorithmsToPlay;
	int index = getRandomNumInRange(0,(algoList.size()-1));
	playerId = algoList[index];
}

int TournamentManager::getRandomNumInRange(int start, int end)
{
	std::random_device rd;								  // obtain a random number from hardware
	std::mt19937 eng(rd());								  // seed the generator
	std::uniform_int_distribution<> yDistr(start, end); // define the range
	return yDistr(eng);
}
