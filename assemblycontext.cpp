//
// Created by eugene on 2019-04-12.
//

//this is a makeshift "symbol table" that will be used to store variable information

#include <iostream>
#include "assemblycontext.h"

using namespace std;

AssemblyContext::AssemblyContext() {
    this->context.numLabels = 0;
    this->context.stackOffset = 0;
}

void AssemblyContext::newScope() {
    this->context.variables.clear();
    this->context.stackOffset = 0; //make 4 (num bytes) a macro later
}

// add a new value to the stack
void AssemblyContext::setOffset(string name, int offset) {
    variableT var;

    var.name = name;
    var.offset = offset;

    context.variables.push_back(var);
}

// return the offset of a variable with the passed in name (if it exists)
int AssemblyContext::getOffset(string name) {
    for (auto it : this->context.variables)
    {
        if (it.name == name)
        {
            return it.offset;
        }
    }
    return -1; // couldn't find var
}