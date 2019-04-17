//
// Created by eugene on 2019-04-12.
//

//this is a makeshift "symbol table" that will be used to store variable information

#include <iostream>
#include "assemblycontext.h"

using namespace std;

AssemblyContext::AssemblyContext() {
    this->context.list = {0};
    this->context.numLabels = 0;
    this->context.stackOffset = 0;
}

void AssemblyContext::newScope() {
    this->context.list = {0};
    this->context.stackOffset -= 4; //make 4 (num bytes) a macro later
}


// add a new value to the stack
void AssemblyContext::fillRegister(string name, int regNum) {
    reg tmpReg;

    tmpReg.name = name;
    tmpReg.registerNum = regNum;

    this->context.list.registers.push_back(tmpReg);
    this->context.list.size++;
}

// return the offset of a variable with the passed in name (if it exists)
int AssemblyContext::getRegister(string name) {
    for (auto it : this->context.list.registers)
    {
        if (it.name == name)
        {
            return it.registerNum;
        }
    }

    cout << "Could not find var " << name << " in the stack." << endl;
    return -1;
}