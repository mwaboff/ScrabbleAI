// Header file for how we represent the board, read in from board.txt

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
using namespace std;

struct Board {
    
    // Fields
    string hand; // Letters in players hand, first line of board.txt
    char board[15][15]; // 15x15 board of characters
        // - is an empty space.
        // capital letter is a tile which has been played.
        // 2 is a double letter tile.
        // 3 is a triple letter tile.
        // 4 is a double word tile.
        // 9 is a triple word tile.
    
    
    // Methods
    Board();
    ~Board();
    bool Empty();
    void Print();
};

#endif