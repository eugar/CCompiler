//
// Created by eugene on 2019-03-25.
//

#ifndef CCOMPILER_IR_H
#define CCOMPILER_IR_H

#include <string>
#include <vector>
#include "parsetree.hpp"

using namespace std;

typedef struct
{
    string op;
    string arg1;
    string arg2;
    string res;
} irInstruction;

class ir {

public:
    ir(ParseTree parseTree);
    void readFromFile(string filename);
    void writeToFile(string filename);
    void generateIR(ParseTree pTree);
    vector <irInstruction> instructions;


private:
    static irInstruction createIns(vector<string> params);
    void getTreeChildren(pnode pn);

};


#endif //CCOMPILER_IR_H
