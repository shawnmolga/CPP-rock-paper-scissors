/*
 * RPSMove.cpp
 *
 *  Created on: 28 Apr. 2018
 *      Author: shawn
 */


#include "RPSMove.h"

RPSMove::RPSMove(RPSpoint from, RPSpoint to) : from(from), to(to){}

//RPSMove::~RPSMove(){}
const Point& RPSMove::getFrom() const{
	return from;
}

const Point& RPSMove::getTo() const{
	return to;
}

void RPSMove::setFrom(RPSpoint point) {
	from = point;
}

void RPSMove::setTo(RPSpoint point) {
	to = point;
}


