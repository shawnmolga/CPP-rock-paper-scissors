/*
 * RPSPiecePosition.cpp
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */
#include "RPSPiecePosition.h"

RPSPiecePosition::RPSPiecePosition (RPSpoint pos, char kind, char rep) :
position(pos), piece(kind), jokerRep(rep){}

virtual const Point& RPSPiecePosition::getPosition() const{
 return position;
}
virtual char RPSPiecePosition::getPiece() const{
	return piece;
}
virtual char RPSPiecePosition::getJokerRep() const{
	jokerRep;
}


