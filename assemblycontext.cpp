//
// Created by eugene on 2019-04-12.
//

//this is a makeshift "symbol table" that will be used to store variable information

#include <iostream>
#include "assemblycontext.h"

using namespace std;

AssemblyContext::AssemblyContext() {
    this->context.registerList = initRegList();
    this->context.numLabels = 0;
    this->context.stackOffset = 0;
}

void AssemblyContext::newScope() {
    this->context.registerList = initRegList();
    this->context.stackOffset -= 4; //make 4 (num bytes) a macro later
}

vector<reg> AssemblyContext::initRegList() {
    vector<reg> registers;
    registers.reserve(8);

    for (int i = 8; i <= 15; i++)
    {
        reg regg;
        regg.registerNum = i;
        regg.name = "";
        regg.age = 0;
        regg.isUsed = 0;
        regg.val = 0;

        registers.push_back(regg);
    }

    return registers;
}

// add a new value to the stack
void AssemblyContext::fillRegister(int regg, string name) {
    //cout << "filling register " << regg << " with " + name << endl;
    this->context.registerList.at(regg - 8).name = name;
    this->context.registerList.at(regg - 8).isUsed = 1;
    this->context.registerList.at(regg - 8).age = 0;
}

// return the offset of a variable with the passed in name (if it exists)
int AssemblyContext::getRegister(string name) {
    for (auto it : this->context.registerList)
    {
        if (it.name == name)
        {
            return it.registerNum;
        }
    }
    return -1; // couldn't find var
}