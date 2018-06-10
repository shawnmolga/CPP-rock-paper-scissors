//
// Created by shawn on 10/06/2018.
//


#include "Piece.h"

Piece::Piece(char playerPiece, bool isPieceJoker) : piece(playerPiece), isJoker(isPieceJoker) {}
Piece::Piece() : piece(0), isJoker(false) {}



char Piece::getPiece() const
{
    return piece;
}

bool Piece::getIsJoker() const
{
    return isJoker;
}
