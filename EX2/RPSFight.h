
#ifndef EX2_RPSFIGHT_H_
#define EX2_RPSFIGHT_H_
#include "FightInfo.h"
#include "Point.h"
#include "RPSpoint.h"

class RPSFight : public FightInfo{
public:
	 RPSFight();
	//~RPSFight();
	virtual const Point& getPosition() const = 0;
	virtual char getOpponentPiece() const = 0; // R, P, S, B or F (but NOT J)
	virtual int getWinner() const = 0; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	void setPosition(RPSpoint pos);
	void setOpponentPiece(char piece);
	void setWinner(int playerNum);
private:
RPSpoint position;
char opponentPiece;
int winner;
};

#endif /* EX2_RPSFIGHT_H_ */
