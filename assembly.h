//
// Created by eugene on 2019-04-11.
//

#ifndef CCOMPILER_ASSEMBLY_H
#define CCOMPILER_ASSEMBLY_H

#include <fstream>
#include "ir.h"
#include "assemblycontext.h"

using namespace std;

class Assembly {

public:
    Assembly(string filename = "out.s");

    void generateCode(vector<irInstruction> instructions);
    void insertBB(irInstruction ins);
    void writeInstruction(string line);
    void chooseInstruction(irInstruction ins);
    void closeOutput(){this->out.close();};
    void writeHeader();
    void writeFunctionPrologue();
    void writeFunctionEpilogue();

private:
    ofstream out;
    int bbcount;
    AssemblyContext assemblyContext;
};


#endif //CCOMPILER_ASSEMBLY_H
