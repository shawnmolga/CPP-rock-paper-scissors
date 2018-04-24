/*
 * RPSpoint.cpp
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */
#include "RPSpoint.h"

RPSpoint::RPSpoint(int x, int y) : x(x), y(y){}
RPSpoint::~RPSpoint(){}
virtual int RPSpoint::getX() const{
	return x;
}
virtual int RPSpoint::getY() const{
	return y;
}
