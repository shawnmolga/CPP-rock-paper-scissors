/*
 * PlayerAlgorithmInfo.cpp
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#include "PlayerAlgorithmInfo.h"

PlayerAlgorithmInfo::PlayerAlgorithmInfo(std::function<std::unique_ptr<PlayerAlgorithm>()> _getPlayerAlgorithm) {
	getPlayerAlgorithm = _getPlayerAlgorithm;
	int score = 0;
	int gamesPlayed = 0;
	(void) score; //to fix compilation error: unused variable
	(void) gamesPlayed; //to fix compilation error: unused variable

}

PlayerAlgorithmInfo::~PlayerAlgorithmInfo() {
	// TODO Auto-generated destructor stub
}

