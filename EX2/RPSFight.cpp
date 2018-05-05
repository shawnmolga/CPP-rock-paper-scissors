/*
 * RPSFight
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSFight.h"

//RPSFight::RPSFight(RPSpoint inputPosition,char inputOpponentPiece, int inputWinner): position(inputPosition),opponentPiece(inputOpponentPiece),winner(inputWinner) {}
RPSFight::RPSFight() : position(RPSpoint(0, 0)), opponentPiece('#'), winner(0) {}

const Point &RPSFight::getPosition() const
{
    return position;
}
char RPSFight::getPiece(int player) const
{
    if (player == 1)
    {
        if (islower(currPiece))
        {
            return currPiece;
        }
        else
        {
            return opponentPiece;
        }
    }
    else
    {
        if (isupper(currPiece))
        {
            return currPiece;
        }
        else
        {
            return opponentPiece;
        }
    }
    return opponentPiece;
}

void RPSFight::setPosition(RPSpoint pos)
{
    position = pos;
}

void RPSFight::setWinner(int playerNum)
{
    winner = playerNum;
}
void RPSFight::setOpponentPiece(char piece)
{
    opponentPiece = piece;
}
void RPSFight::setCurrPiece(char piece)
{
    currPiece = piece;
}