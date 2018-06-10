//
// Created by shawn on 9/06/2018.
//

#ifndef CPP_PRJ_GAMEBOARD_H
#define CPP_PRJ_GAMEBOARD_H


#include <vector>
using namespace std;


using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;
template<typename GAME_PIECE, size_t ROWS, size_t COLS>
class GameBoard<GAME_PIECE>;

template<typename GAME_PIECE, size_t ROWS, size_t COLS>
class GameBoard<GAME_PIECE, ROWS, COLS>{
    size_t NUM_ROWS;
    size_t NUM_COLS;
    std::vector<vector<GAME_PIECE>> GameBoardImp; //implemented GameBoard
    std::vector<vector<int>> GameBoardPlayer ; //contains the player number that owns the piece
    size_t numOfPlayers = 2;


public:
    GameBoard(); //todo write default ctr


    GameBoard(size_t rows, size_t cols, GAME_PIECE) {
        GameBoard(rows, cols, GAME_PIECE, 2);
    }; // todo check - legal overloading?

    GameBoard(size_t rows, size_t cols, GAME_PIECE, size_t numOfPlayers_)
    {
        numOfPlayers = numOfPlayers_;
        NUM_ROWS = rows;
        NUM_COLS = cols;
        vector<vector<GAME_PIECE>> newGameBoard(cols, vector<GAME_PIECE>(rows));
        vector<vector<int>> newPlayerBoard(cols, vector<int>(rows));
        for (int i = 0; i <= rows; i++)
        {
            vector<GAME_PIECE> rowGameVector;
            vector<int> rowPlayerVector;
            for (int j = 0; j <= cols; j++)
            {
                rowGameVector.push_back(nullptr);
                rowPlayerVector.push_back(-1);
            }
            newGameBoard.push_back(rowGameVector);
            newPlayerBoard.push_back(rowPlayerVector);
        }
        GameBoardImp = newGameBoard;
        GameBoardPlayer = newPlayerBoard;
    }


    ~GameBoard(){} ;

    //Get the piece info for the piece in the requested location, or nullptr if no piece in this location.
    PieceInfo<GAME_PIECE> getPiece(int row, int col){
        if ( GameBoardImp.at(row).at(col) == nullptr){
            return nullptr;
        }
        int playerNum = GameBoardPlayers.at(row).at(col);
        auto p1 = std::makePair(playerNum , GameBoardImp.at(row).at(col));
        return std::make_unique<p1>;

    }


    //Method sets the piece into the board.
    //It returns the piece info, for the piece that was previously in this location, or nullptr if there was no piece in this location.
    PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player){
        if (player >= numOfPlayers){
            cout << "ERROR: wrong player number. Did not set." << endl;
            return getPiece(row, col) ;
        }
        auto prevInfo = getPiece( row, col);
        GameBoardImp.at(row).at(col) = piece;
        GameBoardPlayer.at(row).at(col) = player;
        return prevInfo;
    }

    //implement iterator
    PieceInfo<GAME_PIECE> begin(){
        return GameBoardImp.at(0).at(0);
    }

    PieceInfo<GAME_PIECE> end(){
        return GameBoardImp.at(NUM_ROWS).at(NUM_COLS);
    }

    PieceInfo<GAME_PIECE> operator++(){
        //todo
    }

    PieceInfo<GAME_PIECE> operator*(){
        //todo
    }






};



#endif //CPP_PRJ_GAMEBOARD_H
