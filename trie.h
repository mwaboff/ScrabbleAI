#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm> 
using namespace std;

#ifndef TRIE_H
#define TRIE_H

struct Trie {
    // Fields
    map<char, Trie*> children;
    bool isEOW;
    
    // Methods
    bool hasChild(char key);

    bool hasWord(string input, unsigned int offset);
    
    void insert(string input, unsigned int offset);
    
    void LoadDictionary();
    
    Trie();
    
    ~Trie();
    
    void PrintWordsHand(string hand, string string_so_far);
};

#endif