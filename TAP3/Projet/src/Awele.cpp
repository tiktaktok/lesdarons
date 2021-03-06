#include "Awele.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <cstring>

using namespace std;

/**
 * Create a new game of Awele.
 */
Awele::Awele() : Game()
{
    board = new int[12];

    // Fill the 12 holes with 4 seeds
    fill_n(board, 12, 4);
}

/**
 * Destroy a game of Awele.
 */
Awele::~Awele()
{
    this->Game::~Game();
    delete[] board;
}

/**
 * Initialize the Awele game.
 */
void Awele::InitGame()
{
    Game::InitGame();

    // Manual
    std::cout << "Welcome to a new game of Awele!\n" <<
           "When asked for \"Your move:\", enter a number between 0 and 5, " <<
           "where 0 is the cell at the far left of the player.\n" <<
           "The top row cells belong to player 1 and are displayed as if the player " <<
           "was standing on the other side of the board. So his cell '0' is really " <<
           "the one you see at the far right in the top line.\n" <<
           "Have fun!" << std::endl;
}

/**
 * Clone the game
 */
Game* Awele::Clone() const
{
    Awele* game = new Awele();
    for (int i = 0; i < 12; i++) {
        game->board[i] = board[i];
    }
    for (int i=0; i < NB_OF_PLAYERS; i++) {
        game->players[i] = players[i];
        game->score[i] = score[i];
    }
    game->currentPlayerIndex = currentPlayerIndex;
    return game;
}

// GetNextMove provides the next available move for the current game
// GetNextMove will be called as an iterator for the min-max algorithm
char* Awele::GetNextMove(int moveIndex)
{
    if(moveIndex > 5) {
        char* returnVal = (char*) malloc((size_t)(std::strlen("STOP") + 1));
        sprintf(returnVal, "STOP");
        return returnVal;
    }

    // Convert int to char*
    char* move = new char[2]; // moves can be 1 digit (0 - 5);
    sprintf(move, "%d", moveIndex);
    return move;    
}

/**
 * Display a cell with the specified value.
 *
 * @param cellValue the cell value
 */
void Awele::DisplayCellValue(int cellValue, ostream& out) const
{
        out << "[";
		if (cellValue > 0) {
            char* buf = new char[3];
            sprintf(buf, "%2d", cellValue);
            out << buf;
            delete[] buf;
        } else {
            out << "  ";
        }
        out << "] ";
}

/**
 * Display the board on the console.
 */
void Awele::Display() const
{
    // Prints something like this:
    // [ 1] [  ] [ 2] [  ] [ 4] [ 8] <= Player 1, cell 5 4 3 2 1 0      board[] : 11 10  9  8  7  6
    // [  ] [ 2] [ 4] [  ] [10] [12] <= Player 0, cell 0 1 2 3 4 5                 0  1  2  3  4  5
    
    // top player (cells are reversed)
    for (int cell = 11; cell >= 6; cell--)  Awele::DisplayCellValue(board[cell]);
    cout << endl;

    // bottom player
    for (int cell = 0; cell < 6; cell++)    Awele::DisplayCellValue(board[cell]);
    cout << endl;

    // Scores
    cout << "Score player 0: " << GetScore(0) << ",  Score player 1: " << GetScore(1) << endl; 
} // end Display()

/**
 * Returns finished if the game is finished, else returns running.
 *
 * @return finished if game finished, else running
 */
GameStatus Awele::IsFinished() const
{
   return (countSeeds(0) == 0 || countSeeds(1) == 0) ? finished : running;
}

/**
 * Execute the move in the current game.
 *
 * @param move "0" - "5"
 *
 * @return moveOK if move is valid, else badMove
 */
moveStatus Awele::Move(const char * move)
{
    int userInput = atoi(move);
    if (userInput < 0 || userInput > 5) return badMove;

    int cellId = currentPlayerIndex * 6 + userInput;
    if(board[cellId] == 0) return badMove; // empty cell

    int seedsToDistribute = board[cellId];

    board[cellId] = 0; // Empty current cell

    // Perform move
    int nextCell = (cellId+1) % 12;
    while (seedsToDistribute > 0) {
        // there is a rule that say you can't put seeds in the cell you started from.
        if(nextCell == cellId) nextCell = (nextCell+1) % 12;
        board[nextCell]++;
        nextCell = (nextCell+1) % 12;
        seedsToDistribute--;
    }
    
    int lastCell = (nextCell-1) % 12;
    // Take content of cell with 2 or 3 stones
    // but not the ones of the other contestant
    while(
        lastCell >= (((currentPlayerIndex+1)%NB_OF_PLAYERS)*6)+0 &&
        lastCell <= (((currentPlayerIndex+1)%NB_OF_PLAYERS)*6)+5 && 
        (board[lastCell] == 2 || board[lastCell] == 3)) {
        score[currentPlayerIndex] += board[lastCell]; // update score
        board[lastCell] = 0; // empty cell
        lastCell = (nextCell-1) % 12; // go to next cell
    }

    return moveOK;
}

/**
 * Count the seeds of a player
 *
 * @param playerId ID of the player (0 or 1)
 *
 * @return number of seeds of a player
 */
int Awele::countSeeds(int playerId) const // playerId 0 or 1
{
    int offset = playerId * 6;
    int nbSeeds = 0;
    for (int i = 0; i < 6; i++) {
        nbSeeds += board[i+offset];
    }
    return nbSeeds;
}

// Display end of game
void Awele::DisplayEndOfGame() const
{
    using namespace std;
    cout << "Awele is finished." << endl;
    cout << "Scores:" << endl;
    Score score0 = GetScore(0); // real scores accounting for
                                // the last rocks still on the board
    Score score1 = GetScore(1); 
    int remainingSeeds = countSeeds(0) + countSeeds(1);
    score0  += (remainingSeeds+1)/2;
    score1  += (remainingSeeds+1)/2;
    cout << players[0] << ": " << score0 << endl;
    cout << players[1] << ": " << score1 << endl;
}
