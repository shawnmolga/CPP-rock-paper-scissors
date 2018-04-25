/*
 * RPSPiecePosition.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_RPSPIECEPOSITION_H_
#define EX2_RPSPIECEPOSITION_H_
#include "PiecePosition.h"

class RPSPiecePosition : public PiecePosition {
public:
	RPSPiecePosition (RPSpoint pos, char kind, char rep);

	//RPSPiecePosition (Point position, char piece, char jokerRep);
	~RPSPiecePosition();
	virtual const Point& getPosition() const override;
	virtual char getPiece() const override; // R, P, S, B, J or F
	virtual char getJokerRep() const override; // ONLY for Joker: R, P, S or B -- non-Joker may return �#�
private:
	Point position;
	char piece;
	char jokerRep;
};



#endif /* EX2_RPSPIECEPOSITION_H_ */
