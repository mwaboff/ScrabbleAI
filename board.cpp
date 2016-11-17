#include <iostream>
#include <fstream>
#include "board.h"
using namespace std;

Board::Board(){
    ifstream myReadFile;
    myReadFile.open("board.txt");
    if(myReadFile.is_open()){
        getline(myReadFile, hand);
        int collumn = 0;
        int row = 0;
        char letterHolder;
        for(; row < 15; row++){
            for(collumn = 0; collumn < 15; collumn++){
                myReadFile >> noskipws >> letterHolder; // this line of code makes the letter uppercase
                board[row][collumn] = letterHolder;
            }
            myReadFile >> noskipws >> letterHolder; // kill the newline
        }
        myReadFile.close();
    } else cout << "board.txt has failed to open" << endl;
}

Board::~Board(){}

void Board::Print(){
    cout << hand << endl;
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++){
            cout << board[i][j];
        }
        cout << endl;
    }
}

bool Board::Empty(){
    return ((board[7][7] & 64) == 0);
}














