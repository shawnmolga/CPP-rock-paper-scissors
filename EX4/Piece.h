//
// Created by shawn on 10/06/2018.
//

#ifndef CPP_PRJ_PIECE_H
#define CPP_PRJ_PIECE_H


#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
using namespace std;

class Piece
{
public:
    Piece(char piece , bool isJoker);
    Piece();
    char getPiece() const;
    bool getIsJoker() const;

    void setIsJoker(bool isJoker_){
        isJoker = isJoker_;
    }
private:
    char piece;
    bool isJoker;
};


#endif //CPP_PRJ_PIECE_H
