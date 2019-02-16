#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "cscanner.hpp"
#include "cparser.hpp"
#include <vector>
#include <fstream>

using namespace std;
namespace compiler
{
    void compile(string filename);

    vector<Expression> &getExpressions();

    vector<Variable> &getVariables();

    void addVariable(Variable var);

    void printTokens(vector<Token> &tokenList, Scanner &scanner);

    void printTree(Parser &parser);
}
#endif // CCOMPILER_H
