/*
 * RPSJokerChange.c
 *
 *  Created on: 29 Apr. 2018
 *      Author: shawn
 */

#include "RPSJokerChange.h"

RPSJokerChange::RPSJokerChange(char jokerNewRep, RPSpoint jokerChangePosition) : jokerNewRep(jokerNewRep), jokerChangePosition(jokerChangePosition) {}

RPSJokerChange::~RPSJokerChange(){}
const Point& RPSJokerChange::getJokerChangePosition() const{
    return jokerChangePosition;
}

char RPSJokerChange::getJokerNewRep() const{
    return jokerNewRep;
}

void RPSJokerChange::setJokerNewRep(char newRep) {
    jokerNewRep = newRep;
}
void RPSJokerChange::setJokerChangePosition(RPSpoint point){
    jokerChangePosition = point;
}



