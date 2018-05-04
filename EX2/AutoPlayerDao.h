/*
 * AutoPlayerDao.h
 *
 *  Created on: 2 במאי 2018
 *      Author: OR
 */

#ifndef EX2_AUTOPLAYERDAO_H_
#define EX2_AUTOPLAYERDAO_H_

class AutoPlayerDao {
public:
	vector<Vector<int>> getRandomWinningPosition();
private:
	vector<Board> winningBoards;
	void initWinningBoards();
};



#endif /* EX2_AUTOPLAYERDAO_H_ */
