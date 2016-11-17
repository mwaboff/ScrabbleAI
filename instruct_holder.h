#include <iostream>
#include "instruction.h"


#ifndef INSTRUCT_HOLDER_H
#define INSTRUCT_HOLDER_H
using namespace std;

// Priority array

struct InstructHolder {
    
    // Fields
    Instruction* array;
    int size;
    
    // Methods
    InstructHolder();
    ~InstructHolder();
    void insert(Instruction);
    void insert_helper(int, Instruction);
    void print();
};

#endif