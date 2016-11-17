#include <iostream>
#include "instruct_holder.h"
using namespace std;

InstructHolder::InstructHolder(){
    size = 0;
    array = new Instruction[20];
}

InstructHolder::~InstructHolder(){
    delete[] array;
}

void InstructHolder::insert_helper(int i, Instruction instruct){
    Instruction holder = array[i];
    array[i] = instruct;
    for(i += 1; i < size; i++){
        Instruction second_holder = array[i];
        array[i] = holder;
        holder = second_holder;
    }
    return;
}

void InstructHolder::insert(Instruction instruct){
    int i = 0;
    for(; i < size; i++){
        if(instruct.equal(array[i])) return;
        if(array[i].points < instruct.points){
            insert_helper(i, instruct);
            if(size < 20) size++;
            return;
        }
    }
    if (size < 20){
        array[size] = instruct;
        size++;
    }
    return;
}

void InstructHolder::print(){
    for(int i = 0; i < size; i++){
        array[i].print();
    }
    return;
}