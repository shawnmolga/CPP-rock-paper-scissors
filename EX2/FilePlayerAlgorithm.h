/*
 * FilePlayerAlgorithm.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_FILEPLAYERALGORITHM_H_
#define EX2_FILEPLAYERALGORITHM_H_
#include "PlayerAlgorithm.h"
#include "RPSPiecePosition.h"
#include "RPSpoint.h"
#include "FightInfo.h"
#include "dirent.h"
#include "RPSMove.h"
#include "RPSJokerChange.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

class FilePlayerAlgorithm : public PlayerAlgorithm {
public:
    FilePlayerAlgorithm(const string &positionFile, const string &movesFile);
    ~FilePlayerAlgorithm();
    virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) override;
    virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) override;
    virtual void notifyOnOpponentMove(const Move& move) override; // called only on opponent�s move
    virtual void notifyFightResult(const FightInfo& fightInfo) override; // called only if there was a fight
    virtual unique_ptr<Move> getMove() override;
    //todo check if we are returning null in getJokerChange()
    virtual unique_ptr<JokerChange> getJokerChange() override; // nullptr if no change is requested
    virtual int getPieceFromLine(int start, const string &line);
    virtual int getPositionFromLine(int start, const string &line, int &row, int &col);
    virtual bool checkIfDigit(char c);
    bool getPositionAndRepFromLine(const string &line, int &row, int &col, char &jokerRep, char &piece);
    void copyString(string & src, string toCopy);
    bool checkEmptyLine(int start, const string &line);
    unique_ptr<Move> handleNonEmptyLine(string local_line, int & from_x, int & from_y, int & to_x, int & to_y,  RPSpoint& from, RPSpoint& to);
    bool handleJokerPiece(string line,int & nextIndex, char & jokerRep);



private:
    string positionFile;
    string movesFile;
    ifstream player1Move;
    int movesFileLine;
    int nextIndex;
    string line1;

};


#endif



