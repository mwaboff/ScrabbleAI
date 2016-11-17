#include <iostream>
#include "trie.h"
using namespace std;

    
Trie::Trie(){
    isEOW = false;
}

Trie::~Trie(){
    for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
        delete it->second;
    }
}

void Trie::LoadDictionary(){
    ifstream myReadFile;
    myReadFile.open("dictionary.txt");
    string word;
    if (myReadFile.is_open()) {
        while(getline(myReadFile, word)){
            transform(word.begin(), word.end(), word.begin(), ::toupper);    // Convert word to uppercase
            insert(word, 0);
        }
        myReadFile.close();
    } else cout << "dictionary.txt has failed to open" << endl;
    return;
}

bool Trie::hasChild(char key){
    if(key == '*'){
        return children.begin() != children.end();
    }
    map<char, Trie*>::iterator result = children.find(key);
    return result != children.end();      
}




bool Trie::hasWord(string input, unsigned int offset){
    if (input.length() == offset){
        return isEOW;
    }
    
    if(input[offset] == '*'){
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
            if(it->second->hasWord(input, offset+1)){
                return true;
            }
        }
        return false;
    }
    
    if (!hasChild(input[offset])){
        return false;
    }
    
    return (children[input[offset]])->hasWord(input, offset+1);
}

void Trie::insert(string input, unsigned int offset){
    if (input.length() == offset){
        isEOW = true;
        return;
    }
    
    if (!hasChild(input[offset])){
        Trie* newTrie = new Trie();
        children[input[offset]] = newTrie;
    }

    (children[input[offset]])->insert(input, offset+1);
}

void Trie::PrintWordsHand(string hand, string string_so_far){
    
    if (isEOW){
        cout << string_so_far << endl;
    }
    
    if (hand.length() <= 0) return;
    
    for(unsigned int i = 0; i < hand.length(); i++){
        if(hand[i] == '*'){
            
            for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
                string holder_string = hand;
                holder_string.erase(i, 1);
                children[it->first]->PrintWordsHand(holder_string, string_so_far + it->first);
            }
        }
        
        else if(hasChild(hand[i])){
            string holder_string = hand;
            char holder = holder_string[i];
            holder_string.erase(i, 1);
            children[holder]->PrintWordsHand(holder_string, string_so_far + holder);
        }
    }
    
    return;
}























