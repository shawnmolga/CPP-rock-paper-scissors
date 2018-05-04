/*
 * RPSJokerChange.h
 *
 *  Created on: 29 Apr. 2018
 *      Author: shawn
 */

#ifndef EX2_RPSJOKERCHANGE_H_
#define EX2_RPSJOKERCHANGE_H_
#include "JokerChange.h"
#include "Point.h"
#include "RPSpoint.h"
//#endif

class RPSJokerChange : public JokerChange {

public:
	RPSJokerChange(char jokerNewRep, RPSpoint jokerChangePosition);
	~RPSJokerChange();
	virtual Point& getJokerChangePosition() const override;
	virtual char getJokerNewRep() const override; // R, P, S or B (but NOT J and NOT F)
	virtual void setJokerNewRep(char newRep);
	virtual void setJokerChangePosition(RPSpoint point);

private:
	char jokerNewRep;
	RPSpoint jokerChangePosition;
};






#endif /* EX2_RPSJOKERCHANGE_H_ */
