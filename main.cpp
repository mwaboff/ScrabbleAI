#include <iostream>
#include "weights.h"
#include "trie.h"
#include "board.h"
#include <cassert>
#include <fstream>
#include <algorithm> 
#include <string>
using namespace std;

int main(){
    
    Weights weights = Weights();
    assert(weights.Get('A') == 1);
    assert(weights.Get('B') == 3);
    assert(weights.Get('C') == 3);
    assert(weights.Get('D') == 2);
    assert(weights.Get('E') == 1);
    assert(weights.Get('F') == 4);
    assert(weights.Get('G') == 2);
    assert(weights.Get('H') == 4);
    assert(weights.Get('I') == 1);
    assert(weights.Get('J') == 8);
    assert(weights.Get('K') == 5);
    assert(weights.Get('L') == 1);
    assert(weights.Get('M') == 3);
    assert(weights.Get('N') == 1);
    assert(weights.Get('O') == 1);
    assert(weights.Get('P') == 3);
    assert(weights.Get('Q') == 10);
    assert(weights.Get('R') == 1);
    assert(weights.Get('S') == 1);
    assert(weights.Get('T') == 1);
    assert(weights.Get('U') == 1);
    assert(weights.Get('V') == 4);
    assert(weights.Get('W') == 4);
    assert(weights.Get('X') == 8);
    assert(weights.Get('Y') == 4);
    assert(weights.Get('Z') == 10);
    assert(weights.Get('*') == 0);
    
    Trie trie;
    trie.LoadDictionary();
    Board board;
    
    // board.Print();
    
    //assert(board.hand == "LRUREZM");
    ifstream myReadFile;
    myReadFile.open("dictionary.txt");
    string word;
    if (myReadFile.is_open()) {
        while(getline(myReadFile, word)){
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            assert(trie.hasWord(word, 0));
        }
    }
    myReadFile.close();
    
    trie.PrintWordsHand("SCVEKGHA", "");
  
    cout << "All tests passed!" << endl;
}











