/*
 * AlgorithmRegistration.cpp
 *
 *  Created on: 30 ���� 2018
 *      Author: OR
 */

#include "AlgorithmRegistration.h"
#include "TournamentManager.h"

AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    TournamentManager::getTournamentManager().registerAlgorithm(id, factoryMethod);
}

