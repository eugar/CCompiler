//
// Created by eugene on 2019-04-11.
//

#ifndef CCOMPILER_ASSEMBLY_H
#define CCOMPILER_ASSEMBLY_H

#include <fstream>
#include "ir.h"

using namespace std;

class Assembly {

public:
    Assembly(string filename = "out.s");

    void generateCode(vector<irInstruction> instructions);
    void insertBB(irInstruction instruction);
    void outputCode(irInstruction instruction);

private:
    ofstream out;

};


#endif //CCOMPILER_ASSEMBLY_H
