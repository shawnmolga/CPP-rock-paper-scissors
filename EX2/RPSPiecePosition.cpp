/*
 * RPSPiecePosition.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSPiecePosition.h"

RPSPiecePosition::RPSPiecePosition (RPSpoint pos, char kind, char rep) :
position(pos), piece(kind), jokerRep(rep){}

 const Point& RPSPiecePosition::getPosition() const{
 return position;
}
 char RPSPiecePosition::getPiece() const{
	return piece;
}
 char RPSPiecePosition::getJokerRep() const{
	jokerRep;
}


