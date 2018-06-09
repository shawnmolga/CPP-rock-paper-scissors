/*
 * PlayerAlgorithmInfo.h
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#ifndef EX3_PLAYERALGORITHMINFO_H_
#define EX3_PLAYERALGORITHMINFO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <atomic>
#include "PlayerAlgorithm.h"

using namespace std;

class PlayerAlgorithmInfo {
public:
	PlayerAlgorithmInfo(std::function<std::unique_ptr<PlayerAlgorithm>()> _getPlayerAlgorithm);
	std::function<std::unique_ptr<PlayerAlgorithm>()> getPlayerAlgorithm;
	int score;
	atomic<int> gamesPlayed;
};

#endif /* EX3_PLAYERALGORITHMINFO_H_ */
