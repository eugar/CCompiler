//
// Created by eugene on 2019-03-25.
//

#ifndef CCOMPILER_IR_H
#define CCOMPILER_IR_H

#include <string>
#include <vector>
#include "parsetree.hpp"
#include "symboltable.hpp"

using namespace std;

typedef struct irInstruction
{
    // default constructor that zeroes out all values.
    irInstruction()
    : op()
    , arg1()
    , params()
    , arg2()
    , res()
    {}

    string op;
    string arg1;
    pnode *params;
    string arg2;
    string res;
} irInstruction;

class ir {

public:
    ir(ParseTree parseTree, SymbolTable symbolTable);
    void readFromFile(string filename);
    void writeToFile(string filename);
    void generateIR(ParseTree pTree);
    vector <irInstruction> instructions;

    void getGlobals(pnode root);
    void readGlobals();
    void extractGlobals(pnode &root);

private:
    static irInstruction createIns(vector<string> params);
    void getTreeChildren(pnode pn);
    bool generateIr(string rule);
    ParseTree m_parseTree;
    SymbolTable m_symbolTable;
    vector<pnode> m_globals;

};


#endif //CCOMPILER_IR_H
