/*
 * RCPgame.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#include "RCPgame.h"

RCPgame::RCPgame() : isGameOver(false), RCPgaplayerOne(1), playerTwo(2){
	board = new Cell[ROWS][COLS];
}

RCPgame::~RCPgame(){
	for(int i=0; i<ROWS; ++i){
		delete[] board[i];
	}
	delete[] board;
	delete playerOne;
	delete playerTwo;
}

bool RCPgame::fight(int row,int col,char p2Piece, bool isP2PieceJoker){
	//TODO: ask if we can position flag on flag and who is loosing
	char player2Piece=toupper(p2Piece);
	//case 1: 2 players in the same type.
	if(board[row][col].getPiece() == player2Piece){
		switch(board[row][col].getPiece())
		case(board[row][col].getIsJoker()):
		playerTwo.numOfPieces[4]--;
		playerOne.numOfPieces[4]--;
		break;
		case(board[row][col].getPiece() == FLAG):
		playerTwo.numOfPieces[5]--;
		playerOne.numOfPieces[5]--;
		break;
		case(board[row][col].getPiece() == ROCK):
		playerTwo.numOfPieces[0]--;
		playerOne.numOfPieces[0]--;
		break;
		case(board[row][col].getPiece() == PAPER):
		playerTwo.numOfPieces[1]--;
		playerOne.numOfPieces[1]--;
		break;
		case(board[row][col].getPiece() == SCISSOR):
		playerTwo.numOfPieces[2]--;
		playerOne.numOfPieces[2]--;
		break;
		case(board[row][col].getPiece() == BOMB):
		playerTwo.numOfPieces[3]--;
		playerOne.numOfPieces[3]--;
		break;
		Cell::updateCell(board[row][col], 0, false);
	}

	//case 2: player 1 is flag and player 2 another piece
	else if(board[row][col].getPiece() == FLAG){
		if (board[row][col].getIsJoker()){
			playerOne.numOfPieces[4]--;
		}
		else{
			playerOne.numOfPieces[5]--;
		}
		Cell::updateCell(board[row][col], tolower(player2Piece), isP2PieceJoker);
	}
	//case 3: player 2 is flag and player 1 another piece
	else if(player2Piece == FLAG){
		if (board[row][col].getIsJoker()){
			playerTwo.numOfPieces[4]--;
		}
		playerTwo.numOfPieces[5]--;
	}
	//case 2: player 1 is bomb and player 2 another piece
	else if(board[row][col].getPiece() == BOMB){
		if (isP2PieceJoker){
			playerTwo.numOfPieces[4]--;
		}
		else{
			switch (player2Piece){
			case ROCK:
				playerTwo.numOfPieces[0]--;
				break;
			case PAPER:
				playerTwo.numOfPieces[1]--;
				break;
			case SCISSOR:
				playerTwo.numOfPieces[2]--;
				break;
			}
		}

	}
	//case 3: player 2 is bomb and player 1 another piece
	else if(player2Piece == BOMB){
		if (isP2PieceJoker){
			playerOne.numOfPieces[4]--;
		}
		else{
			switch (board[row][col].getPiece()){
			case ROCK:
				playerOne.numOfPieces[0]--;
				break;
			case PAPER:
				playerOne.numOfPieces[1]--;
				break;
			case SCISSOR:
				playerOne.numOfPieces[2]--;
				break;
			}
		}

		board[row][col].getPiece() = tolower(player2Piece);
	}

	else if(board[row][col].getPiece() == PAPER){
		if (player2Piece == ROCK){
			if (isP2PieceJoker){
				playerTwo.numOfPieces[4]--;
			}
			else{
				playerTwo.numOfPieces[0]--;
			}
		}
		else if (player2Piece == SCISSOR){
			if (isP2PieceJoker){
				playerTwo.numOfPieces[4]--;
			}
			else{
				playerOne.numOfPieces[1]--;
			}
			Cell::updateCell(board[row][col], tolower(player2Piece), isP2PieceJoker);
		}
	}

	else if(board[row][col].getPiece() == ROCK){
		if (player2Piece == PAPER){
			if (isP2PieceJoker){
				playerTwo.numOfPieces[4]--;
			}
			else{
				playerOne.numOfPieces[0]--;
			}
			Cell::updateCell(board[row][col], tolower(player2Piece), isP2PieceJoker);
		}
		else if (player2Piece == SCISSOR){
			if (isP2PieceJoker){
				playerTwo.numOfPieces[4]--;
			}
			else{
				playerTwo.numOfPieces[2]--;
			}
		}
	}

	else if(board[row][col].getPiece() == SCISSOR){
		if (player2Piece == PAPER){
			playerTwo.numOfPieces[1]--;
		}
		else if (player2Piece == ROCK){
			playerOne.numOfPieces[0]--;
			Cell::updateCell(board[row][col], tolower(player2Piece), isP2PieceJoker);
		}
	}

	checkGameOver();
}

bool RCPgame::checkGameOver(){
	if (playerOne.numOfPieces[5] == 0 && playerTwo.numOfPieces[5] == 0){
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
		return true;
	}
	//check if all of player one's flags are taken
	if (playerOne.numOfPieces[5] == 0){
		playerTwo.setIsWinner(true);
		playerTwo.setScore(playerTwo.getScore()++);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
	}
	//check if all of player one's moving pieces are eaten
	if (!playerOne.isLeftMovingPieces()){
		playerTwo.setIsWinner(true);
		playerTwo.setScore(playerTwo.getScore()++);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
	}
	//check if all of player two's flags are taken
	if (playerTwo.numOfPieces[5] == 0){
		playerOne.setIsWinner(true);
		playerOne.setScore(playerOne.getScore()++);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
	}

	//check if all of player two's moving pieces are eaten
	if (!playerTwo.isLeftMovingPieces()){
		playerOne.setIsWinner(true);
		playerOne.setScore(playerOne.getScore()++);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
	}

	return false;

}

Player RCPgame::getPlayerOne()const{
	return playerOne;
}

Player RCPgame::getPlayerTwo()const{
	return playerTwo;
}


