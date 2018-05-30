/*
 * PlayerAlgorithmInfo.cpp
 *
 *  Created on: 30 במאי 2018
 *      Author: OR
 */

#include "PlayerAlgorithmInfo.h"

PlayerAlgorithmInfo::PlayerAlgorithmInfo(std::function<std::unique_ptr<PlayerAlgorithm>()> _getPlayerAlgorithm) {
	getPlayerAlgorithm = _getPlayerAlgorithm;
	int score = 0;
	int gamesPlayed = 0;

}

PlayerAlgorithmInfo::~PlayerAlgorithmInfo() {
	// TODO Auto-generated destructor stub
}

