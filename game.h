#include <iostream>
#include "trie.h"
#include "board.h"
#include "weights.h"
#include "instruct_holder.h"
using namespace std;

#ifndef GAME_H
#define GAME_H

struct Game {
    
    // Fields
    Trie* root;
    Board board;
    InstructHolder highest_points;
    Weights weights;
    bool valid_horiz[15][15];
    bool valid_vert[15][15];
    
    
    Game();
    ~Game();
    void Print();

    void HFindValid(int, int);
    void VFindValid(int, int);
    
    void PointsHelper(int x, int y, char c, int &points_so_far, int &multiplier);
    int CountPoints(int x, int y, string word, bool horizontal, string hand);
    void HNewWord(int i, int y, char c, int &extra_points);
    void VNewWord(int x, int i, char c, int &extra_points);
    
    //Legality Functions
    bool CheckHelper(int x, int y, string &temp);
    bool VCheck(int x, int y, char c);
    bool HCheck(int x, int y, char c);
    bool VCheckNoChar(int x);
    bool HCheckNoChar(int y);
    bool VConnected(int x, int y);
    bool HConnected(int x, int y);
    bool IsLetter(int x, int y);
    bool HCheckString(int, int, string);
    bool VCheckString(int, int, string);
    
    // Algorithm funtions
    void MakeWords();
    bool HFoundLetter(char, Trie*, string, int, int, bool, string, char wildcard);
    bool HLetterNotFound(string&, string&, bool&, Trie*&, int, int, int, char wildcard);
    bool VFoundLetter(char, Trie*, string, int, int, bool, string, char wildcard);
    bool VLetterNotFound(string&, string&, bool&, Trie*&, int, int, int, char wildcard);
    void HHelper(int x, int y, string hand, string string_so_far, bool connected, Trie* trie, bool addition, char wildcard);
    void VHelper(int x, int y, string hand, string string_so_far, bool connected, Trie* trie, bool addition, char wildcard);
    // Wildcard
    void HWLetterNotFound(int x, int y, string &hand, string &string_so_far, bool &connected, Trie* &trie, char wildcard);
    void VWLetterNotFound(int x, int y, string &hand, string &string_so_far, bool &connected, Trie* &trie, char wildcard);
   // Empty board
    void HEHelper(int x, int y, string string_so_far, string hand, bool middle, Trie* trie, char wildcard);
    void VEHelper(int x, int y, string string_so_far, string hand, bool middle, Trie* trie, char wildcard);
    void HEWHelper(int x, int y, string handString, string string_so_far, bool middle, Trie* trie, char wildcard);
    void VEWHelper(int x, int y, string handString, string string_so_far, bool middle, Trie* trie, char wildcard);
    
};

#endif