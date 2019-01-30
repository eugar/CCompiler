#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "cscanner.hpp"
#include <vector>
#include <fstream>

using namespace std;

class Compiler
{

public:
    Compiler();
    void compile(string filename);
    vector<Expression> &getExpressions();

    vector<Variable> &getVariables();
    void addVariable(Variable var);
    
    void printTokens();

  private:
    Scanner m_scanner;
    vector<Token> m_tokenList;
    vector<Expression> m_expressions;
    vector<Variable> m_variables;

};

#endif // CCOMPILER_H
