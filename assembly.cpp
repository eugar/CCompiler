//
// Created by eugene on 2019-04-11.
//

#include <iostream>
#include "assembly.h"

using namespace std;

Assembly::Assembly(string filename) {
    this->out.open(filename);

    if (!out.is_open())
    {
        cout << "File" << filename << " could not be opened to write the assembly code file." << endl;
        exit(1);
    }
}

void Assembly::generateCode(vector<irInstruction> instructions) {

}

void Assembly::insertBB(irInstruction instruction) {

}

void Assembly::outputCode(irInstruction instruction) {
    
}