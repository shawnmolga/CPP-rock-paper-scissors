
#ifndef EX2_RPSFIGHT_H_
#define EX2_RPSFIGHT_H_
#include "FightInfo.h"
#include "Point.h"
#include "RPSpoint.h"
#include <ctype.h>

class RPSFight : public FightInfo{
public:

	 RPSFight();
	//~RPSFight();
	const Point& getPosition() const ;
	char  getPiece(int player) const ; // R, P, S, B or F (but NOT J)
 	int getWinner() const ; // 0 - both lost / tie, 1 - player 1 won, 2 - player 2 won
	void setPosition(RPSpoint pos);
	void setOpponentPiece(char piece);
	void setCurrPiece(char piece);
	void setWinner(int playerNum);
private:
RPSpoint position;
char currPiece; //player piece
char opponentPiece; //next player piece
int winner;
};

#endif /* EX2_RPSFIGHT_H_ */
