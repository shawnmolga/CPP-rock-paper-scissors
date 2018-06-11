#include <string>
#include <utility>


#include "GameBoard.h"
#include "unit_test_util.h"


using std::string;
using std::get;

static bool test1(){
    GameBoard<12, 7, string, 4> board;
    int i = 0;
    for (int row = 0; row < 12; row++){
        for(int col =0; col < 7; col++){
            board.setPiece(row, col, "Piece", i);
            i = (i+1)%4;
        }
    }
    int occurence[4]= {0,0,0,0};
    i = 0;
    for(auto pieceInfo : board){
        occurence[std::get<3>(pieceInfo)]++;
    }
    for(int i = 0;i<4;i++){
        if (occurence[i] != 21){
            return false;
        }
    }
    return true;
}

int main(){
    RUN_TEST(test1);
    GameBoard<5, 5, string, 4> myBoard;
    int i =0;
    for (int row = 0; row < 2; row++){
        for(int col =0; col < 5; col++){
            myBoard.setPiece(row, col, "Piece!!", i);
            i = (i+1)%4;
        }
    }
    for(auto pieceInfo : myBoard) {
        cout << "row: " << std::get<0>(pieceInfo);
        cout << " col: " << std::get<1>(pieceInfo);
        cout << " piece: " << std::get<2>(pieceInfo); // we assume here that GAME_PIECE implemented <<
        cout << " player: " << std::get<3>(pieceInfo) << endl;
    }

    for (auto pieceInfo : myBoard.allPiecesOfPlayer(0)){
        cout << "row: " << std::get<0>(pieceInfo);
        cout << " col: " << std::get<1>(pieceInfo);
        cout << " piece: " << std::get<2>(pieceInfo); // we assume here that GAME_PIECE implemented <<
        cout << " player: " << std::get<3>(pieceInfo) << endl;
    }



    return 0;
}
