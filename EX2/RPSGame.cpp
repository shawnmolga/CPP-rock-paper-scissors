
#include "RPSGame.h"
RPSGame::RPSGame() : indexErrorPosOne(0), indexErrorPosTwo(0), indexErrorMoveOne(0), indexErrorMoveTwo(0), isGameOver(false), playerOne(Player(1)), playerTwo(Player(2))
{
    board(ROWS, vector<Cell>(COLS)); //	board = new Cell *[ROWS];
    playerAlgoOne = new FilePlayerAlgorithm(PLAYER_ONE_POSITION_FILENAME, PLAYER_ONE_MOVE_FILENAME);
    playerAlgoTwo = new FilePlayerAlgorithm(PLAYER_TWO_POSITION_FILENAME, PLAYER_TWO_MOVE_FILENAME);
}

RPSGame::~RPSGame()
{
    delete (playerAlgoOne);
    delete (playerAlgoTwo);
}

/*
Update array with relevant piece
Input- playerNum and counter array, and piece 
output- updated array with relevant piece
*/
void RPSGame::countNumOfPieces(const int playerNum, int numOfPositionedPieces[], const int piece)
{
    switch (piece)
    {
    case ROCK:
        numOfPositionedPieces[0]++;
        playerNum == 1 ? playerOne.setNumOfPieces(0,
                                                  playerOne.numOfPieces[0] + 1)
                       : playerTwo.setNumOfPieces(0,
                                                  playerTwo.numOfPieces[0] + 1);
        break;
    case PAPER:
        numOfPositionedPieces[1]++;
        playerNum == 1 ? playerOne.setNumOfPieces(1,
                                                  playerOne.numOfPieces[1] + 1)
                       : playerTwo.setNumOfPieces(1,
                                                  playerTwo.numOfPieces[1] + 1);
        break;
    case SCISSOR:
        numOfPositionedPieces[2]++;
        playerNum == 1 ? playerOne.setNumOfPieces(2,
                                                  playerOne.numOfPieces[2] + 1)
                       : playerTwo.setNumOfPieces(2,
                                                  playerTwo.numOfPieces[2] + 1);
        break;
    case BOMB:
        numOfPositionedPieces[3]++;
        playerNum == 1 ? playerOne.setNumOfPieces(3,
                                                  playerOne.numOfPieces[3] + 1)
                       : playerTwo.setNumOfPieces(3,
                                                  playerTwo.numOfPieces[3] + 1);
        break;
    case JOKER:
        numOfPositionedPieces[4]++;
        playerNum == 1 ? playerOne.setNumOfPieces(4,
                                                  playerOne.numOfPieces[4] + 1)
                       : playerTwo.setNumOfPieces(4,
                                                  playerTwo.numOfPieces[4] + 1);
        break;
    case FLAG:
        numOfPositionedPieces[5]++;
        playerNum == 1 ? playerOne.setNumOfPieces(5,
                                                  playerOne.numOfPieces[5] + 1)
                       : playerTwo.setNumOfPieces(5,
                                                  playerTwo.numOfPieces[5] + 1);
        break;
    default:
        cout << "Error: Bad format - illegal piece" << endl;
    }
}
/*
 Input - array of pieces
 Output - check piece overflow 
*/
bool RPSGame::checkPieceOverflow(int numOfPieces[])
{

    if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
    {
        cout << "Error: a piece type appears in file more than its number"
             << endl;
        return true;
    }
    return false;
}

/*
This function check initial position and locate players on the board.
Input - two boolean fields that indicate if there was a problen in players position format
Output -  false if there is some wrong position
*/
bool RPSGame::checkPositionOnBoard(bool &isPlayerOneLegalFormat, bool &isPlayerTwoLegalFormat,std::vector<unique_ptr<FightInfo>>& fights)
{
    //check if all files exists
    if (!isAllGameFilesExists())
    {
        return false;
    }
    std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerOne;
    std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerTwo;
    playerAlgoOne->getInitialPositions(1, vectorToFillPlayerOne);
    playerAlgoTwo->getInitialPositions(2, vectorToFillPlayerTwo);
    // <vector to fill> contains position(pos), piece(kind), jokerRep(rep)
    //Iterate player 1 vector and position on the board
    int row;
    int col;
    bool isJoker = false;
    int numOfPositionedPieces[6] = {0};
    int flagCnt = 0;
    for (int i = 0; i < vectorToFillPlayerOne.size(); i++)
    {
        isJoker = false;
        int inputPiece = vectorToFillPlayerOne[i]->getPiece();
        // In case the line if bad forrmated
        if (inputPiece == -1)
        {
            isPlayerOneLegalFormat = false;
            return;
        }
        // In case the line could no be read
        else if (inputPiece == -2)
        {
            isPlayerOneLegalFormat = false;
            return;
        }
        else
        {
            //check position was already done there! (maybe we need to trasform here)
            row = vectorToFillPlayerOne[i]->getPosition().getX();
            col = vectorToFillPlayerOne[i]->getPosition().getY();

            if (vectorToFillPlayerOne[i]->getJokerRep() != '#')
            {
                isJoker = true;
            }
            countNumOfPieces(1, numOfPositionedPieces, inputPiece);
            if (toupper(inputPiece) == FLAG)
            {
                flagCnt++;
            }
            if (board[row][col].getPiece() != 0)
            {
                cout
                    << "Error: two or more pieces are positioned on the same location"
                    << endl;
                isPlayerOneLegalFormat = false;
                return false;
            }
            else
            {
                Cell::updateCell(board[row][col], inputPiece, isJoker);
            }
        }
    }
    //check if there are too many pieces positioned on board
    if (checkPieceOverflow(numOfPositionedPieces))
    {
        isPlayerOneLegalFormat = false;
        return false;
    }
    //check if there are not enough flags positioned on board
    if (flagCnt < FLAGS_NUM)
    {
        isPlayerOneLegalFormat = false;
        cout << "Error: Missing flags - not all flags are positioned on board"
             << endl;
        return false;
    }

    //Iterate player 2 vector and position on the board
    flagCnt = 0;
    int numOfPositionedPieces[6] = {0};
    for (int i = 0; i < vectorToFillPlayerTwo.size(); i++)
    {
        isJoker = false;
        int inputPiece = vectorToFillPlayerTwo[i]->getPiece();
        // In case the line if bad forrmated
        if (inputPiece == -1)
        {
            isPlayerTwoLegalFormat = false;
            return false;
        }
        // In case the line could no be read
        else if (inputPiece == -2)
        {
            isPlayerTwoLegalFormat = false;
            return false;
        }
        else
        {
            //check position was already done there! (maybe we need to trasform here)
            row = vectorToFillPlayerTwo[i]->getPosition().getX();
            col = vectorToFillPlayerTwo[i]->getPosition().getY();

            if (vectorToFillPlayerTwo[i]->getJokerRep() != '#')
            {
                isJoker = true;
            }
            countNumOfPieces(2, numOfPositionedPieces, inputPiece);
            if (toupper(inputPiece) == FLAG)
            {
                flagCnt++;
            }
            if (board[row][col].getPiece() != 0)
            {
                if (board[row][col].getPiece() == islower(board[row][col].getPiece()))
                {
                    isPlayerTwoLegalFormat = false;
                    cout
                        << "Error: two or more pieces are positioned on the same location"
                        << endl;
                    return false;
                }
                else
                {
                    fight(true, row, col, tolower(inputPiece), isJoker, fights, vectorToFillPlayerOne[i]->getPosition(), vectorToFillPlayerTwo[i]->getPosition());
                    playerAlgoOne->notifyFightResult(fights);
                    playerAlgoTwo->notifyFightResult(fights);
                }
            }
            else
            {
                Cell::updateCell(board[row][col], tolower(inputPiece), isJoker);
            }
        }
    }
    //check if there are too many pieces positioned on board
    if (checkPieceOverflow(numOfPositionedPieces))
    {
        isPlayerTwoLegalFormat = false;
        return false;
    }
    if (flagCnt < FLAGS_NUM)
    {
        isPlayerTwoLegalFormat = false;
        cout << "Error: Missing flags - not all flags are positioned on board"
             << endl;
        return false;
    }
}

/*
    Reset game result: set player's scroe to 0,  isWinner to false and isGame over to false;
*/
void RPSGame::resetGameResults()
{
    //reset game result after fights that have done due to positioning files
    playerOne.setIsWinner(false);
    playerTwo.setIsWinner(false);
    playerOne.setScore(0);
    playerTwo.setScore(0);
    isGameOver = false;
}
/*
    Set GameOver relevant fields;
    Input - winner number and game over reason
    Output - update player's winning type and score
*/
void RPSGame::setGameOver(int winnerNumber, GAME_OVER_TYPE type)
{
    isGameOver = true;
    gameOverReason = type;
    if (winnerNumber == 0)
    {
        return;
    }

    else if (winnerNumber == playerOne.getPlayerNum())
    {
        playerOne.setIsWinner(true);
        playerOne.setScore(playerOne.getScore() + 1);
        return;
    }
    else
    {
        playerTwo.setIsWinner(true);
        playerTwo.setScore(playerTwo.getScore() + 1);
    }

    return;
}
/*
    This function change game results according to position check file
    Output -  true if game is over due to bad format, otherwise false
*/

bool RPSGame::checkBadFormat()
{
    bool isPlayerOneLegalFormat = true;
    bool isPlayerTwoLegalFormat = true;
    std::vector<RPSFight> fightInfo;
    bool isLegalFormat = checkPositionOnBoard(isPlayerOneLegalFormat, isPlayerTwoLegalFormat,fightInfo);
    resetGameResults();
    if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat)
    {
        setGameOver(0, WRONG_FILE_FORMAT_BOTH);

        return true;
    }
    else if (!isPlayerOneLegalFormat)
    {
        setGameOver(2, WRONG_FILE_FORMAT_ONE);
        return true;
    }

    else if (!isPlayerTwoLegalFormat)
    {
        setGameOver(1, WRONG_FILE_FORMAT_TWO);
        return true;
    }
    playerAlgoOne->notifyOnInitialBoard(board,fightInfo);
    playerAlgoTwo->notifyOnInitialBoard(board,fightInfo);
    return false;
}

/*
parse enum of typeGame and returns 
Input - enum winning type, index line pos of error of player 1, index line pos of error of player 2,index line move of player 1 , index line move of player 2
output - string that represent winning reason.
*/
string RPSGame::ToString(GAME_OVER_TYPE typeGame, int indexErrorPosOne, int indexErrorPosTwo, int indexErrorMoveOne, int indexErrorMoveTwo)
{
    switch (typeGame)
    {
    case ALL_FLAGS_CAPTURED:
        return "All flags of the opponent are captured";
    case ALL_PIECES_EATEN:
        return "All moving PIECEs of the opponent are eaten";
    case WRONG_FILE_FORMAT_ONE:
        return "Bad Positioning input file for player 1 - line " + to_string(indexErrorPosOne); //TODO : line
    case WRONG_FILE_FORMAT_TWO:
        return "Bad Positioning input file for player 2 - line " + to_string(indexErrorPosTwo); //TODO : line
    case WRONG_FILE_FORMAT_BOTH:
        return "Bad Positioning input file for both players - player 1: line " + to_string(indexErrorPosOne) + ", player 2: line " + to_string(indexErrorPosTwo);
    case TIE_NO_WINNER:
        return "A tie - both Moves input files done without a winner";
    case TIE_ALL_FLAGS_EATEN:
        return "A tie - all flags are eaten by both players in the position files";
    case WRONG_MOVE_FILE_FORMAT_TWO:
        return "Bad Move File input file for player 2 - line " + to_string(indexErrorMoveTwo);
    case WRONG_MOVE_FILE_FORMAT_ONE:
        return "Bad Move File input file for player 1 - line " + to_string(indexErrorMoveOne);
    default:
        return "[Unknown GAME_OVER_TYPE]";
    }
}
/*
    Verify that all game files exists
*/
bool RPSGame::isAllGameFilesExists()
{
    bool PositionP1Exists = false, PositioP2Exists = false,
         moveP1Exists = false, moveP2Exists = false;
    std::string currentFilename;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            currentFilename = ent->d_name;
            if (!PositionP1Exists && currentFilename.compare(PLAYER_ONE_POSITION_FILENAME) == 0)
            {
                PositionP1Exists = true;
            }
            else if (!PositioP2Exists && currentFilename.compare(PLAYER_TWO_POSITION_FILENAME) == 0)
            {
                PositioP2Exists = true;
            }
            else if (!moveP1Exists && currentFilename.compare(PLAYER_ONE_MOVE_FILENAME) == 0)
            {
                moveP1Exists = true;
            }
            else if (!moveP2Exists && currentFilename.compare(PLAYER_TWO_MOVE_FILENAME) == 0)
            {
                moveP2Exists = true;
            }
        } //while
        closedir(dir);
        if (!PositionP1Exists || !PositioP2Exists || !moveP1Exists || !moveP2Exists)
        {
            std::cout
                << "Error reading temp dirList file in working directory, Exit from Game."
                << std::endl;
            return false;
        }
        return true;
    }
    else
    {
        std::cout
            << "Unable to open temp dirList file in working directory, Exit from Game."
            << std::endl;
        return false;
    }
    return false;
}
/*
    Return true if game is overm otherwise false
*/
bool RPSGame::checkGameOver(bool isBeforeMove, bool isPlayerOneTurn)
{
    Player *currPlayer = &playerOne;
    Player *nextPlayer = &playerTwo;
    if (!isPlayerOneTurn)
    {
        currPlayer = &playerTwo;
        nextPlayer = &playerOne;
    }
    if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0)
    {
        isGameOver = true;
        currPlayer->setIsWinner(false);
        nextPlayer->setIsWinner(false);
        gameOverReason = TIE_ALL_FLAGS_EATEN;
        return true;
    }
    //check if all of player one's flags are taken
    if (currPlayer->numOfPieces[5] == 0)
    {
        nextPlayer->setIsWinner(true);
        currPlayer->setIsWinner(false);
        nextPlayer->setScore(nextPlayer->getScore() + 1);
        isGameOver = true;
        gameOverReason = ALL_FLAGS_CAPTURED;
        return true;
    }
    //check if all of player two's flags are taken
    if (nextPlayer->numOfPieces[5] == 0)
    {
        currPlayer->setIsWinner(true);
        nextPlayer->setIsWinner(false);
        currPlayer->setScore(currPlayer->getScore() + 1);
        isGameOver = true;
        gameOverReason = ALL_FLAGS_CAPTURED;
        return true;
    }
    //check if all of player one's moving pieces are eaten
    if (isBeforeMove && !currPlayer->isLeftMovingPieces())
    {
        nextPlayer->setIsWinner(true);
        currPlayer->setIsWinner(false);
        nextPlayer->setScore(nextPlayer->getScore() + 1);
        isGameOver = true;
        gameOverReason = ALL_PIECES_EATEN;
        return true;
    }
    return false;
}

bool RPSGame::fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker, std::vector<unique_ptr<FightInfo>> &fights, RPSpoint posOne, RPSpoint posTwo)
{
    Player *currPlayer = &playerOne;
    Player *nextPlayer = &playerTwo;
    RPSpoint *currPos = &posOne;
    RPSpoint *NextPlayerPos = &posTwo;
    int currPlayerNum = 1;
    int nextPlayerNum  = 2;
    if (!isPlayerOneTurn)
    {
        currPlayer = &playerTwo;
        nextPlayer = &playerOne;
        RPSpoint *currPos = &posTwo;
        RPSpoint *NextPlayerPos = &posOne;
        int currPlayerNum = 2;
        int nextPlayerNum  = 1;

    }
    char currPlayerPiece = toupper(currPiece);
    char nextPlayerPiece = toupper(board[row][col].getPiece());

    //Case 1: 2 players in the same type - both should be eaten
    if (nextPlayerPiece == currPlayerPiece)
    {
        switch (nextPlayerPiece)
        {
        case FLAG:
            currPlayer->numOfPieces[5]--;
            nextPlayer->numOfPieces[5]--;
        case ROCK:
            break;
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
        //RPSpoint inputPosition,char inputOpponentPiece, int inputWinner
        fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(), 0));
        Cell::updateCell(board[row][col], 0, false);
    }

    //Case 2: there is flag and current player has another piece
    else if (nextPlayerPiece == FLAG)
    {
        nextPlayer->numOfPieces[5]--;
        if (currPlayerPiece == BOMB)
        {
            //bomb exploded!
            currPlayer->numOfPieces[3]--;
            Cell::updateCell(board[row][col], 0, false);
        }
        else
        {
            Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
        }
        fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),currPlayerNum ));
    }

    //case 3: current player's piece is flag and there is another piece on board
    else if (currPlayerPiece == FLAG)
    {
        currPlayer->numOfPieces[5]--;

        if (nextPlayerPiece == BOMB)
        {
            //bomb exploded!
            nextPlayer->numOfPieces[3]--;
            Cell::updateCell(board[row][col], 0, false);
        }
        fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));
    }
    //case 4: there is bomb and current player has another piece
    else if (nextPlayerPiece == BOMB)
    {
        //bomb exploded!
        nextPlayer->numOfPieces[3]--;
        //piece is eaten
        switch (currPlayerPiece)
        {
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
        Cell::updateCell(board[row][col], 0, false);
        fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));
    }

    //case 5: current player piece is bomb and on board there is another piece
    else if (currPlayerPiece == BOMB)
    {
        //bomb exploded!
        currPlayer->numOfPieces[3]--;

        switch (nextPlayerPiece)
        {
        case ROCK:
            nextPlayer->numOfPieces[0]--;
            break;
        case PAPER:
            nextPlayer->numOfPieces[1]--;
            break;
        case SCISSOR:
            nextPlayer->numOfPieces[2]--;
            break;
        }
        //bomb won and exploded so cell is empty now
        fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));
        Cell::updateCell(board[row][col], 0, false);
    }

    //case 6: there is PAPER and current player has another piece
    else if (nextPlayerPiece == PAPER)
    {
        if (currPlayerPiece == ROCK)
        {
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));
            currPlayer->numOfPieces[0]--;
        }
        else if (currPlayerPiece == SCISSOR)
        {
            nextPlayer->numOfPieces[1]--;
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),currPlayerNum ));
            Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
        }
    }
    //case 7: player 1 is ROCK and player 2 another piece
    else if (nextPlayerPiece == ROCK)
    {
        if (currPlayerPiece == PAPER)
        {
            nextPlayer->numOfPieces[0]--;
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),currPlayerNum ));
            Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
        }
        else if (currPlayerPiece == SCISSOR)
        {
            currPlayer->numOfPieces[2]--;
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));

        }
    }

    else if (nextPlayerPiece == SCISSOR)
    {
        if (currPlayerPiece == PAPER)
        {
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),nextPlayerNum ));
            currPlayer->numOfPieces[1]--;
        }
        else if (currPlayerPiece == ROCK)
        {
            fights.push_back(make_unique<RPSFight>(currPos, board[row][col].getPiece(),currPlayerNum ));
            nextPlayer->numOfPieces[2]--;

            Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
        }
    }
    return checkGameOver(false, isPlayerOneTurn);
}
