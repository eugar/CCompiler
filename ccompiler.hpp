#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "cscanner.hpp"
#include "cparser.hpp"
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
    Parser m_parser;
    vector<Token> m_tokenList;
    vector<Expression> m_expressionList;
    vector<Variable> m_variableList;

};

#endif // CCOMPILER_H
