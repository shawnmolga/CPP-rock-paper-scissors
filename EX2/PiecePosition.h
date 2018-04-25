/*
 * PiecePosition.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_PIECEPOSITION_H_
#define EX2_PIECEPOSITION_H_

#include "Point.h"
#include "RPSBoard.h"
#include "FightInfo.h"
#include "Move.h"
#include "JokerChange.h"
#include "board.h"
#include "RPSpoint.h"

class PiecePosition {
public:
	virtual const Point& getPosition() const;
	virtual char getPiece() const; // R, P, S, B, J or F
	virtual char getJokerRep() const; // ONLY for Joker: R, P, S or B -- non-Joker may return �#�
};


#endif /* EX2_PIECEPOSITION_H_ */
