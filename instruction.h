#include <iostream>
using namespace std;

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

struct Instruction {
    string word;
    int x;
    int y;
    bool right;
    int points;
    char wild;
    
    Instruction(){
        word = "Do Nothing";
        x = 0;
        y = 0;
        right = true;
        points = 0;
        wild = '-';
    }
    
    Instruction(string newWord, int column, int row, bool horizontal, int value, char wildcard){
        word = newWord;
        x = column;
        y = row;
        right = horizontal;
        points = value;
        wild = wildcard;
    };
    
    void print(){
        cout << word << " at (" << x << ", " << y << ") ";
        if(right){
            cout << " horizontally ";
        } else {
            cout << " vertically ";
        }
        cout << "for " << points << " points. ";
        if(wild != '-'){
            cout << "Wildcard is " << wild << ".";
        }
        cout << endl;
    }
    
    bool equal(Instruction i){
        return (word == i.word && x == i.x && y == i.y && right == i.right);
    }
    
    
};

#endif