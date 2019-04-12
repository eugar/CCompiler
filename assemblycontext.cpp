//
// Created by eugene on 2019-04-12.
//

//this is a makeshift "symbol table" that will be used to store variable information

#include <iostream>
#include "assemblycontext.h"

using namespace std;

AssemblyContext::AssemblyContext() {
    this->context.stack;
    this->context.numLabels = 0;
    this->context.stackOffset = 0;
}

void AssemblyContext::newScope() {
    this->context.stack = newStack();
    this->context.stackOffset -= 4; //make 4 (num bytes) a macro later
}

stackItems AssemblyContext::newStack() {
    stackItems stack;
    stack.items;
    stack.size = 0;

    return stack;
}
// add a new value to the stack
void AssemblyContext::setOffset(string name, int offset, stackItems stack) {
    item tmpitem;

    tmpitem.name = name;
    tmpitem.offset = offset;

    stack.items.push_back(tmpitem);
    stack.size++;
}

// return the offset of a variable with the passed in name (if it exists)
int AssemblyContext::getOffset(string name, stackItems stack) {
    for (auto it : stack.items)
    {
        if (it.name == name)
        {
            return it.offset;
        }
    }

    cout << "Could not find var " << name << " in the stack." << endl;
    return -1;
}