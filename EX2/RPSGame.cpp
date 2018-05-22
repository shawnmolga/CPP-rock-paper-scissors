#include "RPSGame.h"

RPSGame::RPSGame() : isGameOver(false), playerOne(Player(1)), playerTwo(Player(2)) {
    gameBoard = RPSBoard();
    playerAlgoOne = NULL;
    playerAlgoTwo = NULL;
}

RPSGame::~RPSGame() {
    delete (playerAlgoOne);
    delete (playerAlgoTwo);
}

void RPSGame::updateJokerMovingPieces() {
    for (int i = 1; i <= COLS; ++i) {
        for (int j = 1; j <= ROWS; ++j) {
            if (gameBoard.board[i][j].getIsJoker()) {
                int pieceIndex;
                switch (toupper(gameBoard.board[i][j].getPiece())) {
                    case ROCK:
                        pieceIndex = 0;
                        break;
                    case PAPER:
                        pieceIndex = 1;
                        break;
                    case SCISSOR:
                        pieceIndex = 2;
                        break;
                    case BOMB:
                        pieceIndex = 3;
                        break;
                }
                isupper(gameBoard.board[i][j].getPiece()) ? playerOne.setNumOfPieces(pieceIndex,
                                                                                     playerOne.numOfPieces[pieceIndex] +
                                                                                     1)
                                                          : playerTwo.setNumOfPieces(pieceIndex,
                                                                                     playerTwo.numOfPieces[pieceIndex] +
                                                                                     1);

            }
        }
    }
}

/*
This function changes joker representation and updates the setNumOfPieces
Input - previous jokerRep, new jokerRep and inidication for current player
 */
void RPSGame::updateJokerChange(char prevJokerRep, char newRep,
                                bool isPlayerOne) {
    //remove one piece from number of prev piece
    switch (prevJokerRep) {
        case ROCK:
            isPlayerOne ? playerOne.setNumOfPieces(0,
                                                   playerOne.numOfPieces[0] - 1)
                        : playerTwo.setNumOfPieces(0,
                                                   playerTwo.numOfPieces[0] - 1);
            break;
        case PAPER:
            isPlayerOne ? playerOne.setNumOfPieces(1,
                                                   playerOne.numOfPieces[1] - 1)
                        : playerTwo.setNumOfPieces(1,
                                                   playerTwo.numOfPieces[1] - 1);
            break;
        case SCISSOR:
            isPlayerOne ? playerOne.setNumOfPieces(2,
                                                   playerOne.numOfPieces[2] - 1)
                        : playerTwo.setNumOfPieces(2,
                                                   playerTwo.numOfPieces[2] - 1);
            break;
        case BOMB:
            isPlayerOne ? playerOne.setNumOfPieces(3,
                                                   playerOne.numOfPieces[3] - 1)
                        : playerTwo.setNumOfPieces(3,
                                                   playerTwo.numOfPieces[3] - 1);
            break;
    }
    //add new piece to number of curr joker piece
    switch (newRep) {
        case ROCK:
            isPlayerOne ? playerOne.setNumOfPieces(0,
                                                   playerOne.numOfPieces[0] + 1)
                        : playerTwo.setNumOfPieces(0,
                                                   playerTwo.numOfPieces[0] + 1);;
            break;
        case PAPER:
            isPlayerOne ? playerOne.setNumOfPieces(1,
                                                   playerOne.numOfPieces[1] + 1)
                        : playerTwo.setNumOfPieces(1,
                                                   playerTwo.numOfPieces[1] + 1);
            break;
        case SCISSOR:
            isPlayerOne ? playerOne.setNumOfPieces(2,
                                                   playerOne.numOfPieces[2] + 1)
                        : playerTwo.setNumOfPieces(2,
                                                   playerTwo.numOfPieces[2] + 1);
            break;
        case BOMB:
            isPlayerOne ? playerOne.setNumOfPieces(3,
                                                   playerOne.numOfPieces[3] + 1)
                        : playerTwo.setNumOfPieces(3,
                                                   playerTwo.numOfPieces[3] + 1);
            break;
    }
}

/*
Input- move object, joker's change object and players turn
onlyOnePlayes - need to know not to notify about opponent moves and fights
Output- returns true if gameOver, false otherwise
 */
bool RPSGame::movePiece(unique_ptr <Move> &move,
                        bool isPlayerOneTurn, bool &isBadFormat, bool onlyOnePlayes) {
    RPSFight fights = RPSFight();
    std::vector <unique_ptr<FightInfo>> initFights;
    bool gameOverIntenral = false;
    bool wasFight = false;
    isBadFormat = false;

    //check if able to move - (maybe all pieces were eaten)
    if (checkGameOver(true, isPlayerOneTurn))
        return true;

    numOfMoves++; // we increment the numerator for each move
    int from_x = move->getFrom().getX(); //col
    int from_y = move->getFrom().getY(); //row
    int to_x = move->getTo().getX();     //col
    int to_y = move->getTo().getY();     //row
    if (!isLegalMove(move, isPlayerOneTurn)) //checking if the move we received is legal
    {
        if (isPlayerOneTurn) {
            setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
        } else
            setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
        isBadFormat = true;
        return true;
    }
    //check if (to_x, to_y) is empty
    if (gameBoard.board[to_x][to_y].getPiece() == 0) {
        Cell::updateCell(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y].getPiece(),
                         gameBoard.board[from_x][from_y].getIsJoker());
        Cell::cleanCell(gameBoard.board[from_x][from_y]);
    } else  //(to_x, to_y) is not empty - need to fight
    {
        //create position from vectors
        RPSpoint Pos1(from_x, from_y);
        RPSpoint Pos2(to_x, to_y);
        if (!isPlayerOneTurn) {
            RPSpoint Pos1(to_x, to_y);
            RPSpoint Pos2(from_x, from_y);
        }
        isGameOver = fight(isPlayerOneTurn, to_x, to_y,
                           gameBoard.board[from_x][from_y].getPiece(),
                           gameBoard.board[from_x][from_y].getIsJoker(), fights, initFights, Pos1,
                           Pos2);
        gameOverIntenral = isGameOver;
        isBadFormat = false;
        Cell::cleanCell(gameBoard.board[from_x][from_y]);
        wasFight = true;
        isPlayerOneTurn ? playerAlgoOne->notifyFightResult(fights) : playerAlgoTwo->notifyFightResult(fights);
    }
    //get JokerChange - if there is no joker-change, should receive null
    unique_ptr <JokerChange> playerJokerChange = (isPlayerOneTurn ? playerAlgoOne->getJokerChange()
                                                                  : playerAlgoTwo->getJokerChange());
    if (playerJokerChange != NULL) {
        int x_joker = playerJokerChange->getJokerChangePosition().getX(); //row
        int y_joker = playerJokerChange->getJokerChangePosition().getY(); //col
        char new_rep = playerJokerChange->getJokerNewRep();
        if (new_rep == 'E') //indicative rep - bad format
        {
            if (isPlayerOneTurn) {
                isBadFormat = true;
                return true;
            }
        } else if (!gameBoard.board[x_joker][y_joker].getIsJoker()) { //if the original peice is not a joker
            cout << "Error: Piece specified is not joker" << endl;
            isBadFormat = true;
            return true;
        }
        if (new_rep != ROCK && new_rep != PAPER && new_rep != SCISSOR && new_rep != BOMB) {
            cout << "Error: illegal NEW_REP " << endl;
            isBadFormat = true;
            return true;
        }
        updateJokerChange(toupper(gameBoard.board[x_joker][y_joker].getPiece()), toupper(new_rep), isPlayerOneTurn);
        Cell::updateCell(gameBoard.board[x_joker][y_joker],
                         isPlayerOneTurn ? new_rep : tolower(new_rep), true);
    }
    if (!onlyOnePlayes) {
        isPlayerOneTurn ? playerAlgoTwo->notifyOnOpponentMove(*move) : playerAlgoOne->notifyOnOpponentMove(*move);
        if (wasFight) {
            isPlayerOneTurn ? playerAlgoTwo->notifyFightResult(fights) : playerAlgoOne->notifyFightResult(fights);
        }
    }
    return gameOverIntenral;
}

//returns -1 if we need to break out of while, 0 otherwise
int RPSGame::handleNonEmptyMove(bool playerOneNonEmpty, std::unique_ptr <Move> &move, bool &isPlayerOneTurn,
                                bool &isGameOverInternal, bool &isBadFormat) {
    if (playerOneNonEmpty)
        isPlayerOneTurn = true;
    else
        isPlayerOneTurn = false;
    isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat, false);
    if (isGameOverInternal) {
        if (isBadFormat) {
            if (playerOneNonEmpty)
                setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
            else
                setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
        }
        return -1;
    }
    return 0;
}
/*
 * returns ERROR_DURING_MOVE (= -1) if we got an error in the OTHER player's move file (the one that isn't empty)
 * otherwise, returns MOVE_DONE_SUCC (= 0)
 */
int RPSGame::handleEOF(bool playerOneEOF, bool &isBadFormat, bool &isPlayerOneTurn, bool &isGameOverInternal,
                       bool isPlayerOneEOF, bool isPlayerTwoEOF) {
    std::unique_ptr <Move> move;
    int nextPlayer = 2;
    if (isPlayerOneEOF) {
        move = playerAlgoTwo->getMove();
        if (move == nullptr)
            isPlayerTwoEOF = true;
    } else {
        nextPlayer = 1;
        move = playerAlgoOne->getMove();
        if (move == nullptr)
            isPlayerOneEOF = true;
    }
    int xPiece;
    if (move != nullptr) {
        xPiece = move->getFrom().getX();
    }
    while (!(nextPlayer == 1 ? isPlayerOneEOF : isPlayerTwoEOF) && xPiece != READ_LINE_ERR &&
           numOfMoves <= MAX_NUM_OF_MOVES) {

        isBadFormat = false;
        if (xPiece != EMPTY_LINE) {
            if (playerOneEOF)
                isPlayerOneTurn = false;
            else
                isPlayerOneTurn = true;
            isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat, true);
            if (isGameOverInternal) {
                if (isBadFormat) {
                    if (playerOneEOF) {
                        setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
                    } else
                        setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
                }
                return ERROR_DURING_MOVE;
            }
        }

        if (isPlayerOneEOF) {
            nextPlayer = 2;
            move = playerAlgoTwo->getMove();
            if (move == nullptr)
                isPlayerTwoEOF = true;
        } else {
            nextPlayer = 1;
            move = playerAlgoOne->getMove();
            if (move == nullptr)
                isPlayerOneEOF = true;
        }
        if (move != nullptr) {
            xPiece = move->getFrom().getX();
        }
    }
    if (xPiece == READ_LINE_ERR) {
        cout << "Error while reading move file. Exiting game" << endl;
        return ERROR_DURING_MOVE;
    }
    return MOVE_DONE_SUCC;
}

/*
 * Output - return -1 if there was an error  during move, or 0 if the move was successful.
 */
int RPSGame::makeMove() {
    bool isPlayerOneEOF = false;
    bool isPlayerTwoEOF = false;
    int xPiecePlayerOne;
    unique_ptr <Move> move = std::move(playerAlgoOne->getMove());
    if (move == nullptr) {
        isPlayerOneEOF = true;
    } else {
        xPiecePlayerOne = move->getFrom().getX();
    }
    int xPiecePlayerTwo = 1;
    bool isPlayerOneTurn = true;
    bool isGameOverInternal = false;
    bool isBadFormat = false;

    while (!isPlayerOneEOF && xPiecePlayerOne != READ_LINE_ERR && !isPlayerTwoEOF && xPiecePlayerTwo != READ_LINE_ERR &&
           numOfMoves <= MAX_NUM_OF_MOVES)
    {
        isBadFormat = false;
        isGameOverInternal = false;
        if (xPiecePlayerOne != 0) {
            if (-1 == handleNonEmptyMove(true, move, isPlayerOneTurn, isGameOverInternal, isBadFormat))
                break;
        }
        move = std::move(playerAlgoTwo->getMove());
        if (move == nullptr) {
            isPlayerTwoEOF = true;
        } else {
            xPiecePlayerTwo = move->getFrom().getX();
        }
        if (!isPlayerTwoEOF && xPiecePlayerTwo != READ_LINE_ERR) {
            if (xPiecePlayerTwo != EMPTY_LINE) {
                if (-1 == handleNonEmptyMove(false, move, isPlayerOneTurn, isGameOverInternal, isBadFormat))
                    break;
            }
        }
        else if (xPiecePlayerTwo == READ_LINE_ERR) {
            return ERROR_DURING_MOVE;
        } else {
            break;
        }

        move = playerAlgoOne->getMove();
        if (move == nullptr) {
            isPlayerOneEOF = true;
        } else {
            xPiecePlayerOne = move->getFrom().getX();
        }
    }
    if (-1 == closeGame(isGameOverInternal, isBadFormat, isPlayerOneTurn, isPlayerOneEOF, isPlayerTwoEOF))
        return -1;
    return MOVE_DONE_SUCC;
}

int RPSGame::closeGame(bool &isGameOverInternal, bool &isBadFormat, bool &isPlayerOneTurn, bool isPlayerOneEOF,
                       bool isPlayerTwoEOF) {

    if (numOfMoves >= 100) {
        setGameOver(0, TIE_NO_WINNER);
        gameOverReason = TOO_MANY_MOVES;
        return GAME_OVER_SUCC;
    }

    if (!isGameOverInternal) {
        if (isPlayerOneEOF) //EOFcase player 1
        {
            if (-1 == handleEOF(true, isBadFormat, isPlayerOneTurn, isGameOverInternal, isPlayerOneEOF, isPlayerTwoEOF))
                return ERROR_DURING_MOVE;
        } else if (isPlayerTwoEOF) //player 2 eof
        {
            if (-1 ==
                handleEOF(false, isBadFormat, isPlayerOneTurn, isGameOverInternal, isPlayerOneEOF, isPlayerTwoEOF))
                return ERROR_DURING_MOVE;
        }

        if (numOfMoves >= 100) {
            setGameOver(0, TIE_NO_WINNER);
            gameOverReason = TOO_MANY_MOVES;
            return GAME_OVER_SUCC;
        }

        isPlayerOneTurn = !isPlayerOneTurn;
        bool doesGameOver = checkGameOver(true, isPlayerOneTurn);
        if (!isGameOverInternal && !isGameOver && !doesGameOver) {
            if (isPlayerOneTurn) {
                if (!playerTwo.isLeftMovingPieces()) {
                    setGameOver(1, ALL_PIECES_EATEN);
                    return GAME_OVER_SUCC;
                }
            } else {
                if (!playerOne.isLeftMovingPieces()) {
                    setGameOver(2, ALL_PIECES_EATEN);
                    return GAME_OVER_SUCC;
                }
            }
            setGameOver(0, TIE_NO_WINNER);
        }
    }
    return GAME_OVER_SUCC;
}

/*
 *  Return BAD_FORMAT_POS_ERR (= -1) if there is bad format error
 *  or READ_LINE_POS_ERR (= -2) if there is getline error
 *  and GAME_OVER_SUCC ( = 0) if everything is ok
 */
int RPSGame::startGame() {
    int initResult = checkBadFormat();
    if (initResult == BAD_FORMAT_POS_ERR) {
        return BAD_FORMAT_POS_ERR;
    } else if (initResult == READ_LINE_POS_ERR) {
        cout << "Error: could not read line in file. exiting..." << endl;
        return READ_LINE_POS_ERR;
    }
    bool isPlayerOneTurn = true;
    bool isAboutToMove = true;
    if (checkGameOver(isAboutToMove, isPlayerOneTurn)) {
        return GAME_OVER_SUCC;
    }
    //If we got here the board is initialized! now we need to make a move.
    return makeMove();
}

bool RPSGame::isLegalMove(unique_ptr <Move> &move, bool isPlayer1) {
    int from_x = move->getFrom().getX();
    int from_y = move->getFrom().getY();
    int to_x = move->getTo().getX();
    int to_y = move->getTo().getY();
    //checks if all coordinates are within the scope of the board
    if ((from_x < 1 || from_x > COLS) || (to_x < 1 || to_x > COLS) || (from_y < 1 || from_y > ROWS) ||
        (to_y < 1 || to_y > ROWS)) {
        cout << "Error: illegal location on board" << endl;
        return false;
    }
    //checks  if the Move object we got actually moves an object on the board
    if (from_x == to_x && from_y == to_y) {
        cout << "Error: user MUST move one piece" << endl;
        return false;
    }
    //check to see if there is a piece in the sepcified coordinates in the Move object
    if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0) {
        cout << "Error: there is no piece in this position" << endl;
        return false;

    }
    //checks whether the piece we want to move belongs to the player from who we received the move
    else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece())) ||
               (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece()))) {
        cout << "Error: trying to move the opponent piece" << endl;
        return false;
    }
    //checks whether the piece we want to move is allowed to move by the rules of the game
    if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB ||
        toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG) {
        cout << "Error: flag/bomb piece is not allowed to move" << endl;
        return false;
    }
    //player's pieces are only allowed to move one cell up, down, left or right
    if (!checkDirectionOfMove(from_x, from_y, to_x, to_y))
        return false;
    //checks if the Move object is trying to move to a coordinate on the board taken by it's own player's piece
    if (!checkIfCellTaken(isPlayer1, to_x, to_y))
        return false;

    return true;
}

/*
 * returns false if (to_x, to_y) is taken by the player's peice
 * returns true otherwise
 */
bool RPSGame::checkIfCellTaken(bool isPlayer1, int to_x, int to_y) {
    if (gameBoard.board.at(to_x).at(to_y).getPiece() != 0) {
        if (isPlayer1) {
            if (isupper(gameBoard.board.at(to_x).at(to_y).getPiece())) {
                cout
                        << "Error: you are trying to move to a cell taken by your own piece"
                        << endl;
                return false;
            }
        } else {
            if (islower(gameBoard.board.at(to_x).at(to_y).getPiece())) {
                cout
                        << "Error: you are trying to move to a cell taken by your own piece"
                        << endl;
                return false;
            }
        }
    }
    return true;
}

/*
 * returns true if we are trying to move one cell up, down, left or right
 * returns false otherwise
 */
bool RPSGame::checkDirectionOfMove(int from_x, int from_y, int to_x, int to_y) {

    if (to_x == from_x + 1 || to_x == from_x - 1) {
        if (to_y != from_y) {
            cout
                    << "Error: illegal move - can move only one cell up/down/left/right "
                    << endl;
            return false;
        }
    } else if (to_y == from_y + 1 || to_y == from_y - 1) {
        if (to_x != from_x) {
            cout
                    << "Error: illegal move - can move only one cell up/down/left/right"
                    << endl;
            return false;
        }
    } else {
        cout
                << "Error: illegal move - can move only one cell up/down/left/right"
                << endl;
        return false;
    }
    return true;
}

/*
 * Update array with relevant piece
 * Input- playerNum and counter array, and piece
 * output- updated array with relevant piece , return true is piece is ok otherwise false!
 */
bool RPSGame::countNumOfPieces(const int playerNum, int numOfPositionedPieces[],
                               const int piece) {
    switch (piece) {
        case ROCK:
            numOfPositionedPieces[0]++;
            playerNum == 1 ? playerOne.setNumOfPieces(0, playerOne.numOfPieces[0] + 1) : playerTwo.setNumOfPieces(0,
                                                                                                                  playerTwo.numOfPieces[0] +
                                                                                                                  1);
            return true;
            break;
        case PAPER:
            numOfPositionedPieces[1]++;
            playerNum == 1 ? playerOne.setNumOfPieces(1, playerOne.numOfPieces[1] + 1) : playerTwo.setNumOfPieces(1,
                                                                                                                  playerTwo.numOfPieces[1] +
                                                                                                                  1);
            return true;
            break;
        case SCISSOR:
            numOfPositionedPieces[2]++;
            playerNum == 1 ? playerOne.setNumOfPieces(2, playerOne.numOfPieces[2] + 1) : playerTwo.setNumOfPieces(2,
                                                                                                                  playerTwo.numOfPieces[2] +
                                                                                                                  1);
            return true;
            break;
        case BOMB:
            numOfPositionedPieces[3]++;
            playerNum == 1 ? playerOne.setNumOfPieces(3, playerOne.numOfPieces[3] + 1) : playerTwo.setNumOfPieces(3,
                                                                                                                  playerTwo.numOfPieces[3] +
                                                                                                                  1);
            return true;
            break;
        case JOKER:
            numOfPositionedPieces[4]++;
            playerNum == 1 ? playerOne.setNumOfPieces(4, playerOne.numOfPieces[4] + 1) : playerTwo.setNumOfPieces(4,
                                                                                                                  playerTwo.numOfPieces[4] +
                                                                                                                  1);
            return true;
            break;
        case FLAG:
            numOfPositionedPieces[5]++;
            playerNum == 1 ? playerOne.setNumOfPieces(5, playerOne.numOfPieces[5] + 1) : playerTwo.setNumOfPieces(5,
                                                                                                                  playerTwo.numOfPieces[5] +
                                                                                                                  1);
            return true;
            break;
        default:
            cout << "Error: Bad format - illegal piece" << endl;
            return false;
    }
}

/*
 Input - array of pieces
 Output - check piece overflow
 */
bool RPSGame::checkPieceOverflow(int numOfPieces[]) {
    if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM ||
        numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM) {
        cout << "Error: a piece type appears in file more than its number"
             << endl;

        return true;
    }
    return false;
}

int RPSGame::checkNumOfPieces(int playerNum, bool isPieceOkPlayer1, bool isPieceOkPlayer2, int inputPiece,
                              bool &isPlayerLegalFormat, int numOfPositionedPieces[]) {
    if (playerNum == 1) {
        isPieceOkPlayer1 = countNumOfPieces(1, numOfPositionedPieces, inputPiece);
        if (!isPieceOkPlayer1) {
            isPlayerLegalFormat = false;
            return -1;
        }
    } else {
        isPieceOkPlayer2 = countNumOfPieces(2, numOfPositionedPieces, inputPiece);
        if (!isPieceOkPlayer2) {
            isPlayerLegalFormat = false;
            return -1;
        }
    }

    return 0;
}

/*
 *  receives coordinates x, y and inputPiece, and updates the cell with that piece on the board.
 *  if it's player ones turn and two pieces are in the same cell - return -1
 *  if it's player two's turn and two pieces are in the same cell -
 *  return -1 if the cell is taken by player two's piece
 *  otherwise fight
 */
int RPSGame::insertToBoard(int playerNum, int x, int y, char inputPiece, bool &isPlayerLegalFormat, bool &isJoker,
                           unique_ptr <PiecePosition> &piecePos, RPSFight &fights,
                           std::vector <unique_ptr<FightInfo>> &initFights) {
    if (playerNum == 1) {
        if (gameBoard.board.at(x).at(y).getPiece() != 0) {
            cout
                    << "Error: two or more pieces are positioned on the same location for player " << playerNum << endl;
            isPlayerLegalFormat = false;
            return -1;
        } else {
            if (isJoker)
                Cell::updateCell(gameBoard.board.at(x).at(y), piecePos->getJokerRep(), isJoker);
            else
                Cell::updateCell(gameBoard.board.at(x).at(y), inputPiece, isJoker);
        }
    } else {
        if (gameBoard.board.at(x).at(y).getPiece() != 0) {
            if (gameBoard.board.at(x).at(y).getPiece() == tolower(gameBoard.board.at(x).at(y).getPiece())) {
                isPlayerLegalFormat = false;
                cout << "Error: two or more pieces are positioned on the same location for player " << playerNum
                     << endl;
                return -1;
            } else {
                RPSpoint player1Pos(piecePos->getPosition().getX(), piecePos->getPosition().getY()); //noy changes!
                RPSpoint player2Pos(piecePos->getPosition().getX(), piecePos->getPosition().getY()); //noy changes
                if (isJoker) {
                    fight(false, x, y, tolower(piecePos->getJokerRep()), isJoker, fights, initFights,
                          player1Pos, player2Pos);
                } else
                    fight(false, x, y, tolower(inputPiece), isJoker, fights, initFights,
                          player1Pos, player2Pos);
            }
        } else {
            if (isJoker)
                Cell::updateCell(gameBoard.board.at(x).at(y), tolower(piecePos->getJokerRep()), isJoker);
            else
                Cell::updateCell(gameBoard.board.at(x).at(y), tolower(inputPiece),
                                 isJoker);
        }
    }
    return 0;
}

/*
 This function locate players on the board.
 Input - player number, vectorTofill (that vector contain all players pieces), isPlayerLegalFormat will indicate if there s bad format
 numOFPieces array in order to check if all the pieces are located in the board, fights and initFights vector (for notify function)
 Output : BAD_FORMAT_POS_ERR(-1) if there is bad format on player one or READ_LINE_POS_ERR (-2) if there is a problem with getline func
 */
int
RPSGame::locateOnBoard(int playerNum, std::vector <unique_ptr<PiecePosition>> &vectorToFill, bool &isPlayerLegalFormat,
                       int numOfPositionedPieces[], RPSFight &fights, std::vector <unique_ptr<FightInfo>> &initFights) {
    int vectorSize = vectorToFill.size();
    bool isJoker = false;
    int x;
    int y;
    //initialize temporary board to check player's positions do not collide
    char board[COLS][ROWS] = {0};
    int inputPiece;
    int flagCnt = 0;
    bool isPieceOkPlayer1 = true;
    bool isPieceOkPlayer2 = true;
    for (int i = 0; i < vectorSize; i++) {
        isJoker = false;
        inputPiece = toupper(vectorToFill[i]->getPiece());
        // In case the line if bad forrmated
        if (inputPiece == BAD_FORMAT_POS_ERR) {
            isPlayerLegalFormat = false;
            return BAD_FORMAT_POS_ERR;
        } else if (inputPiece == READ_LINE_POS_ERR) {
            isPlayerLegalFormat = false;
            return READ_LINE_POS_ERR;
        } else {
            //check position was already done there! (maybe we need to trasform here)
            x = vectorToFill[i]->getPosition().getX();
            y = vectorToFill[i]->getPosition().getY();
            if (vectorToFill[i]->getJokerRep() != '#')
                isJoker = true;

            if (-1 == checkNumOfPieces(playerNum, isPieceOkPlayer1, isPieceOkPlayer2, inputPiece, isPlayerLegalFormat,
                                       numOfPositionedPieces))
                return -1;
            //position is illegal - tried to locate 2 pieces of same player in same position
            if (board[x - 1][y - 1] != 0) {
                cout << "Error: two or more pieces are positioned on the same location"
                     << endl;
                isPlayerLegalFormat = false;
                return BAD_FORMAT_POS_ERR;
            }
            board[x - 1][y - 1] = inputPiece;
            if (toupper(inputPiece) == FLAG)
                flagCnt++;
            if (insertToBoard(playerNum, x, y, inputPiece, isPlayerLegalFormat, isJoker, vectorToFill[i], fights,
                              initFights) == -1) {
                return BAD_FORMAT_POS_ERR;
            }
        }
    }

    //check if there are too many pieces positioned on board
    if (checkPieceOverflow(numOfPositionedPieces)) {
        isPlayerLegalFormat = false;
        return BAD_FORMAT_POS_ERR;
    }
    //check if there are not enough flags positioned on board
    if (flagCnt < FLAGS_NUM) {
        isPlayerLegalFormat = false;
        cout << "Error: Missing flags - not all flags are positioned on board for player " << playerNum << endl;
        return BAD_FORMAT_POS_ERR;
    }
    return NO_BAD_FORMAT_ERR;
}

/*
 This function check initial position and locate players on the board.
 Input - two boolean fields that indicate if there was a problen in players position format
 Output -
 BAD_FORMAT_POS_ERR(-1) if there is an error
 BAD_FORMAT_POS_ERR(-2) if there is an read error
 NO_BAD_FORMAT_ERR (0)  if there is no bad formats problem or errors.
 */
int RPSGame::checkPositionOnBoard(bool &isPlayerOneLegalFormat,
                                  bool &isPlayerTwoLegalFormat,
                                  RPSFight &fights, std::vector <unique_ptr<FightInfo>> &initFights) {
    std::vector <unique_ptr<PiecePosition>> vectorToFillPlayerOne;
    std::vector <unique_ptr<PiecePosition>> vectorToFillPlayerTwo;

    playerAlgoOne->getInitialPositions(1, vectorToFillPlayerOne);
    playerAlgoTwo->getInitialPositions(2, vectorToFillPlayerTwo);

    int numOfPositionedPieces[6] = {0};

    //check player One Format
    int resultPlayerOne = locateOnBoard(1, vectorToFillPlayerOne, isPlayerOneLegalFormat, numOfPositionedPieces, fights,
                                        initFights);
    updateJokerMovingPieces();
    memset(numOfPositionedPieces, 0, sizeof(numOfPositionedPieces)); // for automatically-allocated arrays
    int resultPlayerTwo = locateOnBoard(2, vectorToFillPlayerTwo, isPlayerTwoLegalFormat, numOfPositionedPieces, fights,
                                        initFights);
    updateJokerMovingPieces();
    if (resultPlayerOne == BAD_FORMAT_POS_ERR || resultPlayerOne == READ_LINE_POS_ERR) {
        return BAD_FORMAT_POS_ERR;
    } else if (resultPlayerTwo == READ_LINE_POS_ERR || resultPlayerTwo == BAD_FORMAT_POS_ERR) {
        return READ_LINE_POS_ERR;
    }
    return NO_BAD_FORMAT_ERR;
}

/*
 Reset game result: set player's scroe to 0,  isWinner to false and isGame over to false;
 */
void RPSGame::resetGameResults() {
    //reset game result after fights that have done due to positioning files
    playerOne.setIsWinner(false);
    playerTwo.setIsWinner(false);
    playerOne.setScore(0);
    playerTwo.setScore(0);
    isGameOver = false;
}

/*
 * Set GameOver relevant fields;
 * Input - winner number and game over reason
 * Output - update player's winning type and score
 */
void RPSGame::setGameOver(int winnerNumber, GAME_OVER_TYPE type) {
    isGameOver = true;
    gameOverReason = type;
    if (winnerNumber == 0) {
        return;
    } else if (winnerNumber == playerOne.getPlayerNum()) {
        playerOne.setIsWinner(true);
        playerTwo.setIsWinner(false);
        playerOne.setScore(playerOne.getScore() + 1);
        return;
    } else {
        playerTwo.setIsWinner(true);
        playerOne.setIsWinner(false);
        playerTwo.setScore(playerTwo.getScore() + 1);
    }

    return;
}

/*
 * checks if the position files are legal and if not - updates the game score/reason
 */
int RPSGame::checkBadFormat() {
    bool isPlayerOneLegalFormat = true;
    bool isPlayerTwoLegalFormat = true;
    RPSFight fightInfo = RPSFight();

    std::vector <unique_ptr<FightInfo>> initFights;
    int isLegalFormat = checkPositionOnBoard(isPlayerOneLegalFormat,
                                             isPlayerTwoLegalFormat, fightInfo, initFights);
    resetGameResults(); //reset the game result as we did in ex1!
    if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat) {
        setGameOver(0, WRONG_FILE_FORMAT_BOTH);
        return -1;
    } else if (!isPlayerOneLegalFormat) {
        setGameOver(2, WRONG_FILE_FORMAT_ONE);
        return -1;
    } else if (!isPlayerTwoLegalFormat) {
        setGameOver(1, WRONG_FILE_FORMAT_TWO);
        return -1;
    }
    const RPSBoard gameBoardConst = gameBoard;
    playerAlgoOne->notifyOnInitialBoard(gameBoardConst, initFights);
    playerAlgoTwo->notifyOnInitialBoard(gameBoardConst, initFights);
    return isLegalFormat;
}

/*
 * parse enum of typeGame and returns
 * Input - enum winning type, index line pos of error of player 1, index line pos of error of player 2,index line move of player 1 , index line move of player 2
 * output - string that represent winning reason.
 */
string RPSGame::ToString(GAME_OVER_TYPE typeGame) {
    switch (typeGame) {
        case ALL_FLAGS_CAPTURED:
            return "All flags of the opponent are captured";
        case ALL_PIECES_EATEN:
            return "All moving PIECEs of the opponent are eaten";
        case WRONG_FILE_FORMAT_ONE:
            return "Bad Positioning input file for player 1 "; //TODO : line
        case WRONG_FILE_FORMAT_TWO:
            return "Bad Positioning input file for player 2 "; //TODO : line
        case WRONG_FILE_FORMAT_BOTH:
            return "Bad Positioning input file for both players - player 1 , player 2";
        case TIE_NO_WINNER:
            return "A tie - both Moves input files done without a winner";
        case TIE_ALL_FLAGS_EATEN:
            return "A tie - all flags are eaten by both players in the position files";
        case WRONG_MOVE_FILE_FORMAT_TWO:
            return "Bad Moves input file for player 2  ";
        case WRONG_MOVE_FILE_FORMAT_ONE:
            return "Bad Moves input file for player 1  ";
        case TOO_MANY_MOVES:
            return "Number of moves exceeds maximum";
        default:
            return "[Unknown GAME_OVER_TYPE]";
    }
}

/*
 * Verify that all game files exists
 */
bool RPSGame::isAllGameFilesExists(bool isPlayerOneUseFile, bool isPlayerTwoUseFile) {
    bool PositionP1Exists = false, PositioP2Exists = false,
            moveP1Exists = false, moveP2Exists = false;
    std::string currentFilename;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            currentFilename = ent->d_name;
            if (!PositionP1Exists && currentFilename.compare(PLAYER_ONE_POSITION_FILENAME) == 0)
                PositionP1Exists = true;

            else if (!PositioP2Exists && currentFilename.compare(PLAYER_TWO_POSITION_FILENAME) == 0)
                PositioP2Exists = true;

            else if (!moveP1Exists && currentFilename.compare(PLAYER_ONE_MOVE_FILENAME) == 0)
                moveP1Exists = true;

            else if (!moveP2Exists && currentFilename.compare(PLAYER_TWO_MOVE_FILENAME) == 0)
                moveP2Exists = true;

        } //while
        closedir(dir);
        if (isPlayerOneUseFile && (!PositionP1Exists || !moveP1Exists)) {
            std::cout
                    << "Error reading temp dirList file in working directory, Exit from Game."
                    << std::endl;
            return false;
        }
        if (isPlayerTwoUseFile && (!PositioP2Exists || !moveP2Exists)) {
            std::cout
                    << "Error reading temp dirList file in working directory, Exit from Game."
                    << std::endl;
            return false;
        }
        return true;
    } else {
        std::cout
                << "Unable to open temp dirList file in working directory, Exit from Game."
                << std::endl;
        return false;
    }
    return false;
}

void
RPSGame::gameOverSetter(Player *currPlayer, Player *nextPlayer, bool isWinner1, bool isWinner2, GAME_OVER_TYPE type) {
    isGameOver = true;
    currPlayer->setIsWinner(isWinner1);
    nextPlayer->setIsWinner(isWinner2);
    gameOverReason = type;
    if (isWinner1)
        currPlayer->setScore(currPlayer->getScore() + 1);
    if (isWinner2)
        nextPlayer->setScore(currPlayer->getScore() + 1);
    return;
}

/*
 * Return true if game is over otherwise false
 */
bool RPSGame::checkGameOver(bool isBeforeMove, bool isPlayerOneTurn) {
    Player *currPlayer = &playerOne;
    Player *nextPlayer = &playerTwo;
    if (!isPlayerOneTurn) {
        currPlayer = &playerTwo;
        nextPlayer = &playerOne;
    }
    if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0) {
        gameOverSetter(currPlayer, nextPlayer, false, false, TIE_ALL_FLAGS_EATEN);
        return true;
    }
    //check if all of player one's flags are taken
    if (currPlayer->numOfPieces[5] == 0) {
        gameOverSetter(currPlayer, nextPlayer, false, true, ALL_FLAGS_CAPTURED);
        return true;
    }
    //check if all of player two's flags are taken
    if (nextPlayer->numOfPieces[5] == 0) {
        gameOverSetter(currPlayer, nextPlayer, true, false, ALL_FLAGS_CAPTURED);
        return true;
    }
    //check if all of player one's moving pieces are eaten
    if (isBeforeMove && !currPlayer->isLeftMovingPieces()) {
        gameOverSetter(currPlayer, nextPlayer, false, true, ALL_PIECES_EATEN);

        return true;
    }
    return false;
}

void
RPSGame::handleSwitchCase(char nextPlayerPiece, RPSFight &fights, Player *currPlayer, Player *nextPlayer, int x, int y,
                          std::vector <unique_ptr<FightInfo>> &initFights, RPSFight *ptr) {
    switch (nextPlayerPiece) {
        case FLAG:
            currPlayer->numOfPieces[5]--;
            nextPlayer->numOfPieces[5]--;
            break;
        case ROCK:
            currPlayer->numOfPieces[0]--;
            nextPlayer->numOfPieces[0]--;
            break;
        case PAPER:
            currPlayer->numOfPieces[1]--;
            nextPlayer->numOfPieces[1]--;
            break;
        case SCISSOR:
            currPlayer->numOfPieces[2]--;
            nextPlayer->numOfPieces[2]--;
            break;
        case BOMB:
            currPlayer->numOfPieces[3]--;
            nextPlayer->numOfPieces[3]--;
            break;
    }
    fights.setWinner(0);
    ptr->setWinner(0);
    initFights.push_back(make_unique<RPSFight>(*ptr));
    Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
}

void
RPSGame::handleNextPlayerPieceFlag(bool isCurrPlayerFlag, char currPlayerPiece, Player *currPlayer, Player *nextPlayer,
                                   int x, int y,
                                   std::vector <unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
                                   bool isCurrPieceJoker, char currPiece, RPSFight &fights, int currPlayerNum) {
    nextPlayer->numOfPieces[5]--;
    if (currPlayerPiece == BOMB) {
        currPlayer->numOfPieces[3]--;
        Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
    } else if (!isCurrPlayerFlag) {
        Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
                         isCurrPieceJoker);
    }

    fights.setWinner(currPlayerNum);
    ptr->setWinner(currPlayerNum);
    initFights.push_back(make_unique<RPSFight>(*ptr));
}


void RPSGame::handleNexPlayerBomb(char currPlayerPiece, Player *currPlayer, Player *nextPlayer, int x, int y,
                                  std::vector <unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
                                  RPSFight &fights, int nextPlayerNum) {
    nextPlayer->numOfPieces[3]--;    //piece is eaten
    switch (currPlayerPiece) {
        case ROCK:
            currPlayer->numOfPieces[0]--;
            break;
        case PAPER:
            currPlayer->numOfPieces[1]--;
            break;
        case SCISSOR:
            currPlayer->numOfPieces[2]--;
            break;
    }
    //bomb won and exploded so cell is empty now
    Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
    fights.setWinner(nextPlayerNum);
    ptr->setWinner(nextPlayerNum);
    initFights.push_back(make_unique<RPSFight>(*ptr));
}

void RPSGame::handlePaperVsRock(Player *currPlayer,
                                std::vector <unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
                                RPSFight &fights, int nextPlayerNum) {

    fights.setWinner(nextPlayerNum);
    ptr->setWinner(nextPlayerNum);
    initFights.push_back(make_unique<RPSFight>(*ptr));
    currPlayer->numOfPieces[0]--;
}

void RPSGame::handlePaperOrRockVsScissors(int i, Player *nextPlayer, int currPlayerNum,
                                          std::vector <unique_ptr<FightInfo>> &initFights, RPSFight *ptr,
                                          RPSFight &fights) {
    nextPlayer->numOfPieces[i]--;
    fights.setWinner(currPlayerNum);
    ptr->setWinner(currPlayerNum);
    initFights.push_back(make_unique<RPSFight>(*ptr));
}

/*
 * Input - boolean that represents players turn, pieces row and col of next player ,
 * curr piece player,  if the player is joker or not, RPSFight object for updating fight result
 * and vector to update for initial fights, 2 points that represent players position.
 * Output - true if game if over, otherwise false
 */
bool RPSGame::fight(bool isPlayerOneTurn, int x, int y, char currPiece,
                    bool isCurrPieceJoker, RPSFight &fights, std::vector <unique_ptr<FightInfo>> &initFights,
                    RPSpoint posOne, RPSpoint posTwo) {
    numOfMoves = 0; // we resent the numerator if there is a fight
    Player *currPlayer = &playerOne;
    Player *nextPlayer = &playerTwo;
    RPSpoint *currPos = &posOne;
    RPSpoint *NextPlayerPos = &posTwo;
    int currPlayerNum = 1;
    int nextPlayerNum = 2;
    RPSFight *ptr = new RPSFight(); //check this - might be bad practice
    if (!isPlayerOneTurn) {
        currPlayer = &playerTwo;
        nextPlayer = &playerOne;
        *currPos = posTwo;
        *NextPlayerPos = posOne;
        currPlayerNum = 2;
        nextPlayerNum = 1;
    }
    char currPlayerPiece = toupper(currPiece);
    char nextPlayerPiece = toupper(gameBoard.board.at(x).at(y).getPiece());
    ptr->setPlayerOnePiece(currPlayerNum == 1 ? currPlayerPiece : nextPlayerPiece);
    ptr->setPlayerTwoPiece(nextPlayerNum == 1 ? currPlayerPiece : nextPlayerPiece);
    ptr->setPosition(*NextPlayerPos);

    fights.setPosition(*NextPlayerPos);
    fights.setPlayerOnePiece(ptr->getPiece(1));
    fights.setPlayerTwoPiece(ptr->getPiece(2));

    //Case 1: 2 players in the same type - both should be eaten
    if (nextPlayerPiece == currPlayerPiece)
        handleSwitchCase(nextPlayerPiece, fights, currPlayer, nextPlayer, x, y, initFights, ptr);

    //Case 2: there is flag and current player has another piece
    else if (nextPlayerPiece == FLAG)
        handleNextPlayerPieceFlag(false, currPlayerPiece, currPlayer, nextPlayer, x, y,
                                  initFights, ptr, isCurrPieceJoker, currPiece, fights, currPlayerNum);

    //case 3: current player's piece is flag and there is another piece on board
    else if (currPlayerPiece == FLAG)
        handleNextPlayerPieceFlag(true, nextPlayerPiece, nextPlayer, currPlayer, x, y,
                                  initFights, ptr, isCurrPieceJoker, currPiece, fights, nextPlayerNum);

    //case 4: there is bomb and current player has another piece
    else if (nextPlayerPiece == BOMB)
        handleNexPlayerBomb(currPlayerPiece, currPlayer, nextPlayer, x, y, initFights, ptr,
                            fights, nextPlayerNum);

    //case 5: current player piece is bomb and on board there is another piece
    else if (currPlayerPiece == BOMB)
        handleNexPlayerBomb(nextPlayerPiece, nextPlayer, currPlayer, x, y, initFights, ptr,
                            fights, currPlayerNum);

    //case 6: there is PAPER and current player has another piece
    else if (nextPlayerPiece == PAPER) {
        if (currPlayerPiece == ROCK)
            handlePaperVsRock(currPlayer, initFights, ptr, fights, nextPlayerNum);

        else if (currPlayerPiece == SCISSOR) {
            handlePaperOrRockVsScissors(1, nextPlayer, currPlayerNum,  initFights, ptr, fights);
            Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
                             isCurrPieceJoker);
        }
    }
    //case 7: player 1 is ROCK and player 2 another piece
    else if (nextPlayerPiece == ROCK) {
        if (currPlayerPiece == PAPER) {
            handlePaperVsRock(nextPlayer, initFights, ptr, fights, currPlayerNum);
            Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
                             isCurrPieceJoker);
        } else if (currPlayerPiece == SCISSOR)
            handlePaperOrRockVsScissors(2, currPlayer, nextPlayerNum,  initFights, ptr,  fights);
    } else if (nextPlayerPiece == SCISSOR) {
        if (currPlayerPiece == PAPER) {
            handlePaperOrRockVsScissors(1, currPlayer, nextPlayerNum, initFights, ptr, fights);
            //fights.setPosition(*currPos);
        } else if (currPlayerPiece == ROCK) {
            handlePaperOrRockVsScissors(2, nextPlayer, currPlayerNum, initFights, ptr, fights);
            Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
                             isCurrPieceJoker);
        }
    }
    free(ptr);
    return checkGameOver(false, isPlayerOneTurn);
}

/*
 Input - ofstream of output file
 Output - print the board to the file
 */
void RPSGame::printBoardToFile(ofstream &output) {
    for (int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {
            if (gameBoard.board[j][i].getIsJoker()) {
                if (Cell::isPlayerOnePiece(gameBoard.board[j][i])) {
                    output << "J";
                } else {
                    output << "j";
                }
            } else if (gameBoard.board[j][i].getPiece() == 0) {
                output << " ";
            } else {
                output << gameBoard.board[j][i].getPiece();
            }
        }
        output << endl;
    }
    output.ofstream::close();
    return;
}

/*
* This function prints the board to the file.
* Input - output file name
*/
void RPSGame::printOutputFile(const string &outputFile) {
    ofstream output;
    output.open(outputFile, ios::trunc);
    if (output.fail()) {
        cout << "Error while opening output file. Exiting game" << endl;
        return;
    }
    //player 1 is thw winner

    if (playerOne.getIsWinner()) {
        output << "Winner: 1" << endl;
    }
        //player 2 is the winner
    else if (playerTwo.getIsWinner()) {
        output << "Winner: 2" << endl;
    }
        //tie
    else {
        output << "Winner: 0" << endl;
    }
    output << "Reason: "
           << ToString(getGameOverReason())
           << endl;
    output << endl; // the third line must be en empty line!
    printBoardToFile(output);
    output.close();
    return;
}

/*
* Input - game method.
* Output- true if we successfuly init playerAlogrithms or false otherwise;
 */
bool RPSGame::initGameMethod(string gameMethod) {
    bool isPlayerOneUseFile = false;
    bool isPlayerTwoUseFile = false;
    if (gameMethod.compare("auto-vs-file") == 0) {
        isPlayerTwoUseFile = true;
    } else if (gameMethod.compare("file-vs-auto") == 0) {
        isPlayerOneUseFile = true;
    } else if (gameMethod.compare("auto-vs-auto") == 0) {
        return initPlayersAlgo(isPlayerOneUseFile, isPlayerTwoUseFile);
    } else if (gameMethod.compare("file-vs-file") == 0) {
        isPlayerOneUseFile = true;
        isPlayerTwoUseFile = true;
    } else {
        cout << "Error: Illegal game method." << endl;
        return false;
    }
    return initPlayersAlgo(isPlayerOneUseFile, isPlayerTwoUseFile);
}

/*
* Input - two boolean parameters that indicated if the players are using file play mode
* Output- true if we successfuly init playerAlogrithms or false otherwise;
*/
bool RPSGame::initPlayersAlgo(bool isPlayerOneUseFile, bool isPlayerTwoUseFile) {
    if (isPlayerOneUseFile || isPlayerTwoUseFile) {
        if (!isAllGameFilesExists(isPlayerOneUseFile, isPlayerTwoUseFile))
            return false;
    }

    if (isPlayerOneUseFile) {
        playerAlgoOne = new FilePlayerAlgorithm(PLAYER_ONE_POSITION_FILENAME, PLAYER_ONE_MOVE_FILENAME);
    } else {
        playerAlgoOne = new AutoPlayerAlgorithm();
    }
    if (isPlayerTwoUseFile) {
        playerAlgoTwo = new FilePlayerAlgorithm(PLAYER_TWO_POSITION_FILENAME, PLAYER_TWO_MOVE_FILENAME);
    } else {
        playerAlgoTwo = new AutoPlayerAlgorithm();
    }

    return true;
}

//Need to erase this function - only for debug!
void RPSGame::PrintBoardToConsole() {
    cout << "*******************PRINT THE BOARD:****************" << endl;
    for (int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {
            if (gameBoard.board[j][i].getIsJoker()) {
                if (Cell::isPlayerOnePiece(gameBoard.board[j][i])) {
                    cout << " J ";
                } else {
                    cout << " j ";
                }
            } else if (gameBoard.board[j][i].getPiece() == 0) {
                cout << " - ";
            } else {
                cout << " " << gameBoard.board[j][i].getPiece() << " ";
            }
        }
        cout << endl;
    }
}
