/*
 * RPSpoint.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSpoint.h"

RPSpoint::RPSpoint(int x, int y) : x(x), y(y){}
int RPSpoint::getX() const{
	return x;
}
int RPSpoint::getY() const{
	return y;
}
void RPSpoint::setX(int new_x){
	x = new_x;
}
void RPSpoint::setY(int new_y){
	y = new_y;
}

