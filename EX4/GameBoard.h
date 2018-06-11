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
        string type = "cell";
        Cell(){
            piece = nullptr;
            player = -1;
        }

    };
    std::vector<vector<Cell>> GameBoardImp; //implemented GameBoard


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
        GameBoardImp = newGameBoard;
    };

    ~GameBoard(){} ;

    //Get the piece info for the piece in the requested location, or nullptr if no piece in this location.
    PieceInfo getPiece(int row, int col){
        if ( GameBoardImp.at(row).at(col).piece == nullptr){
            return nullptr;
        }
        int playerNum = GameBoardImp.at(row).at(col).player;
        auto p1 = std::make_pair(playerNum , *GameBoardImp.at(row).at(col).piece);
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
        GameBoardImp.at(row).at(col).piece = &piece;
        GameBoardImp.at(row).at(col).player = player;
        return prevInfo;
    }

    //////////////////////////ITERATOR://////////////////////
    class iterator{
    public:

        int currRow = 0;
        int currCol = 0;
        std::vector<vector<Cell>> iterBoard;
        iterator(int currRow_, int currCol_, const std::vector<vector<Cell>> & board_) : currRow(currRow_), currCol(currCol_) , iterBoard(board_){
            cout << "in iterator constructor" << endl;
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

        virtual iterator& operator++() {
            bool cont = incrementIndex();//if we need to continue - true
            while (iterBoard.at(currRow).at(currCol).piece == nullptr && cont ){
                cont = incrementIndex();
            }
            return *this;
        }

        virtual bool operator==(iterator other) const {
            if (currRow == other.currRow && currCol == other.currCol)
                return true;
            else
                return false;
        }

        virtual bool operator!=(iterator other) const {
            return !(*this == other);
        }

        virtual const std::tuple<int, int, GAME_PIECE, int>  operator*() const {
            GAME_PIECE currPiece;
            if (iterBoard.at(currRow).at(currCol).piece == nullptr){
                currPiece = nullptr;
            }
            else{
                currPiece = *iterBoard.at(currRow).at(currCol).piece;
            }
            int player = iterBoard.at(currRow).at(currCol).player;
            return std::make_tuple(currRow, currCol, currPiece, player);
        }

        iterator findLastPlayerPiece() {
        	iterator tmpItr = playerIter(0,0,GameBoardImp);
        	for (;tmpItr != GameBoardImp.end(); ++tmpItr) {
        		if (++tmpItr == GameBoardImp.end()) {
        			break;
        		}
        		++(*this);
        	}

        	return *this;
        }
    };

    iterator begin() {
        return iterator(0,0, GameBoardImp);
    }

    iterator end() {
    	iterator it(0,0,GameBoardImp);
        return it.findLastPlayerPiece();
    }


    class playerPieces {
    public:

        int playerNum;

        playerPieces(int playerNum_) : playerNum(playerNum_){}

        //explicit playerIterator(int currRow_, int currCol_, const std::vector<vector<Cell>> & board_) : currRow(currRow_), currCol(currCol_) , iterBoard(board_){}
        class playerIter {
        public:
        	playerIter(){}
        	iterator iter = iterator(0,0,GameBoardImp);
        	playerIter operator++() {
        		for (;iter != GameBoardImp.end(); ++iter) {
        			if (playerNum == (*iter)[3]) {
        				return *this;
        			}
        		}
        	}
        	bool operator!=(playerIter other) const {
        		return !(*this == other);
        	}

        	bool operator==(playerIter other) const{
        		return *this==other;
        	}

        	const std::tuple<int, int, GAME_PIECE, int>  operator*() const {
        		return *iter;
        	}

        	playerIter findLastPlayerPiece() {
        		playerIter tmpItr = playerIter(0,0,GameBoardImp);
        		for (;tmpItr != GameBoardImp.end(); ++tmpItr) {
        			if (++tmpItr == GameBoardImp.end()) {
        				break;
        			}
        			++(*this);
        		}

        		return *this;
        	}
        };

        playerIter begin() {
               playerIter pItr;
               return pItr++;
           }

           playerIter end() {
        	   playerIter pItr;
               return pItr.findLastPlayerPiece();
           }

    };

    playerPieces allPiecesOfPlayer(int playerNum) {
    	return playerPieces(playerNum);
    }

};



#endif //CPP_PRJ_GAMEBOARD_H
