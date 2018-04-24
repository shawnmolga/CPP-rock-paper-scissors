/*
 * PiecePosition.h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_PIECEPOSITION_H_
#define EX2_PIECEPOSITION_H_

#include "Point.h"


class PiecePosition {
public:
	virtual const Point& getPosition() const;
	virtual char getPiece() const; // R, P, S, B, J or F
	virtual char getJokerRep() const; // ONLY for Joker: R, P, S or B -- non-Joker may return ‘#’
};


#endif /* EX2_PIECEPOSITION_H_ */
