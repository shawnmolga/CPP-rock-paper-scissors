//
// Created by shawn on 9/06/2018.
//

#ifndef CPP_PRJ_GAMEBOARD_H
#define CPP_PRJ_GAMEBOARD_H


#include <vector>
#include <memory>
#include <iostream>
#include <tuple>

using namespace std;

template<int ROWS, int COLS, class GAME_PIECE, int NUM_PLAYERS = 2>
class GameBoard{
    using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

    struct Cell{
        GAME_PIECE* piece;
        int player;
        Cell(){
            piece = nullptr;
            player = -1;
        }

        ~Cell(){
        	delete piece;
        }

    };
    std::vector<vector<Cell>>gameBoardImp; //implemented GameBoard


public:
    GameBoard(){
        vector<vector<Cell>> newGameBoard(COLS, vector<Cell>(ROWS));
        for (int i = 0; i <= ROWS; i++)
        {
            vector<Cell> rowGameVector;
            for (int j = 0; j <= COLS; j++)
                rowGameVector.push_back(Cell());
            newGameBoard.push_back(rowGameVector);
        }
        gameBoardImp = newGameBoard;
    };

    ~GameBoard(){} ;

    //Get the piece info for the piece in the requested location, or nullptr if no piece in this location.
    PieceInfo getPiece(int row, int col){
        if (gameBoardImp.at(row).at(col).piece == nullptr){
            return nullptr;
        }
        int playerNum = gameBoardImp.at(row).at(col).player;
        auto p1 = std::make_pair(playerNum , *(gameBoardImp.at(row).at(col).piece));
        return std::make_unique<pair<int, GAME_PIECE>>(p1);
    }


    //Method sets the piece into the board.
    //It returns the piece info, for the piece that was previously in this location, or nullptr if there was no piece in this location.
    PieceInfo setPiece(int row, int col, GAME_PIECE piece, int player){
        if (player >= NUM_PLAYERS){
            std::cout << "ERROR: wrong player number. Did not set Piece." << endl;
            return getPiece(row, col) ;
        }
        auto prevInfo = getPiece( row, col);
        gameBoardImp.at(row).at(col).piece = new GAME_PIECE(piece);
        //cout <<"PIECE IS: "<<*(gameBoardImp.at(row).at(col).piece)<<endl;
        gameBoardImp.at(row).at(col).player = player;
        return prevInfo;
    }

    //////////////////////////ITERATOR://////////////////////
    class iterator{
    public:
        int currRow;
        int currCol;
        GameBoard* board;
        int playerNum;
        GAME_PIECE* pieceItr;
        iterator(int currRow_, int currCol_, GameBoard* board_, int playerNum_, GAME_PIECE* piece_) :
        	currRow(currRow_), currCol(currCol_) , board(board_), playerNum(playerNum_), pieceItr(piece_){
            cout << "in iterator constructor" << endl;
        }
        iterator(GameBoard* board_, int playerNum_, GAME_PIECE* piece_) :
        	board(board_), playerNum(playerNum_), pieceItr(piece_){
        	currRow = 0;
        	currCol = 0;
        	bool cont = true;
        	while ((board->gameBoardImp.at(currRow).at(currCol).piece == nullptr ? true : (isNotDesiredPiece() || isNotPlayerPiece())) && cont ){

        		cont = incrementIndex();
        	}
        	cout << "in begin iterator constructor" << endl;
        }
        bool incrementIndex(){
            if (currCol < COLS-1 && currRow < ROWS -1){
                currCol++;
                return true;
            }
            else if (currCol == COLS -1  && currRow < ROWS -1  ){
                currCol = 0;
                currRow++;
                return true;
            }
            else if (currCol<COLS -1  && currRow == ROWS -1 ) {
                currCol++;
                return true;
            }
            else if (currCol == COLS-1 && currRow == ROWS -1){
                currRow++;
                currCol++;
                return false;
            }
            return false;//todo: should not get here!
        }

        bool isNotPlayerPiece() {
        	if (playerNum < 0)
        		return false;
        	return (board->gameBoardImp.at(currRow).at(currCol).player != playerNum);
        }

        bool isNotDesiredPiece() {
        	if (pieceItr == nullptr)
        		return false;
        	return (*board->gameBoardImp.at(currRow).at(currCol).piece != *pieceItr);
        }

         iterator& operator++() {
            bool cont = incrementIndex();//if we need to continue - true

            while ((board->gameBoardImp.at(currRow).at(currCol).piece == nullptr ? true : (isNotDesiredPiece() || isNotPlayerPiece())) && cont ){

            	cont = incrementIndex();
            }
            return *this;
        }

        bool operator==(iterator other) const {
            if (currRow == other.currRow && currCol == other.currCol)
                return true;
            else
                return false;
        }

        bool operator!=(iterator other) const {
            return !(*this == other);
        }

         const std::tuple<int, int, GAME_PIECE, int>  operator*() const {
            int player = board->gameBoardImp.at(currRow).at(currCol).player;
            return std::make_tuple(currRow, currCol, *board->gameBoardImp.at(currRow).at(currCol).piece, player);
        }

    };

    iterator begin() {
        return iterator(this, -1, nullptr);
    }

    iterator end() {
    	return iterator(ROWS,COLS, this, -1, nullptr);
    }



    ///////////////////////////////PLAYER ITERATOR///////////////////
    class IterFactory {
    public:
    	GAME_PIECE* desiredPiece;
    	int desiredPlayerNum;
        int row;
        int col;
        GameBoard* desiredBoard;


        IterFactory(GAME_PIECE* piece_, int numPlayer_, int row_, int col_, GameBoard *board_) :
        	desiredPiece(piece_), desiredPlayerNum(numPlayer_) , row(row_) , col(col_), desiredBoard(board_) {}

        ~IterFactory() {
        	delete desiredPiece;
        }

        iterator begin() {
            return iterator(desiredBoard, desiredPlayerNum, desiredPiece);
        }

        iterator end() {
            return iterator(ROWS,COLS, desiredBoard, desiredPlayerNum, desiredPiece);
        }
    };

    IterFactory allPiecesOfPlayer(int playerNum){
            return IterFactory(nullptr,playerNum,0,0, this);
    }

    IterFactory allOccureneceOfPiece(GAME_PIECE piece){
    	GAME_PIECE* p = new GAME_PIECE(piece);
    	return IterFactory(p,-1,0,0, this);
    }

    IterFactory allOccureneceOfPieceForPlayer(GAME_PIECE piece, int playerNum){
    	GAME_PIECE* p = new GAME_PIECE(piece);
    	return IterFactory(p,playerNum,0,0, this);
    }
};
    
#endif //CPP_PRJ_GAMEBOARD_H
