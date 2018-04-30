/*
 * RPSFight
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSFight.h"

RPSFight::RPSFight(RPSpoint inputPosition,char inputOpponentPiece, int inputWinner): position(inputPosition),opponentPiece(inputOpponentPiece),winner(inputWinner) {}

const Point& RPSFight::getPosition() const{
    return position;
}
char RPSFight::getOpponentPiece() const {
    return opponentPiece;
}
