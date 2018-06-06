/*
 * PlayerAlgorithmInfo.cpp
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#include "PlayerAlgorithmInfo.h"

PlayerAlgorithmInfo::PlayerAlgorithmInfo(std::function<std::unique_ptr<PlayerAlgorithm>()> _getPlayerAlgorithm) {
	getPlayerAlgorithm = _getPlayerAlgorithm;
	score = 0;
	gamesPlayed = 0;
	}
