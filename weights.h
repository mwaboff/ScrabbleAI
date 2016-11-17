#include <iostream>
#include <fstream>
using namespace std;

#ifndef WEIGHTS_H
#define WEIGHTS_H

struct Weights {
    
    int* weight_array;
    
    Weights(){
        weight_array = new int[27];
        int num;
        int counter = 0;
        ifstream myfile("weights.txt");
        if(myfile.is_open()){
            while(myfile >> num){
                weight_array[counter] = num;
                counter++;
            }
            myfile.close();
        } else {
            cout << "weights.txt failed to open" << endl;
        }
    }
    
    ~Weights(){
        delete[] weight_array;
    }
    
    char Hash(char scrabbleChar){
        scrabbleChar -= 'A';
        if(scrabbleChar > 25){
            return 26;
        } else {
            return scrabbleChar;
        }
    }
    
    int Get(char scrabbleChar){
        return weight_array[Hash(scrabbleChar)];
    }
    
    int operator [](char scrabbleChar){
        return weight_array[Hash(scrabbleChar)];
        
    }
    
    
};

#endif
