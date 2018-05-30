/*
 * RPSFight
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSFight.h"

//RPSFight::RPSFight(RPSpoint inputPosition,char inputOpponentPiece, int inputWinner): position(inputPosition),opponentPiece(inputOpponentPiece),winner(inputWinner) {}
RPSFight::RPSFight() : position(RPSpoint(0, 0)),playerOnePiece('#') ,playerTwoPiece('#'), winner(0) {}

const Point &RPSFight::getPosition() const
{
    return position;
}
char RPSFight::getPiece(int player) const
{
    if (player == 1)
    {
        return playerOnePiece;
    }
    else
    {
        return playerTwoPiece;
    }
}

void RPSFight::setPosition(RPSpoint pos)
{
    position = pos;
}

void RPSFight::setWinner(int playerNum)
{
    winner = playerNum;
}
void RPSFight::setPlayerOnePiece(char piece)
{
    playerOnePiece = piece;
}
void RPSFight::setPlayerTwoPiece(char piece)
{
    playerTwoPiece = piece;
}
int  RPSFight::getWinner()const
{
    return winner;
}
