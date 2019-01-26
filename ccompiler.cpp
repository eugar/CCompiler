#include "ccompiler.hpp"
#include <iostream>

using namespace std;

/**
 * [Compiler::Compiler Compiler contructor]
 */
Compiler::Compiler(){
    scanner = Scanner();
}

/**
 * [Compiler::getExpressions gets the expressions generated
 * by the scanner to eventually get interpreted]
 * @return [a vector of expressions]
 */
vector<Expression> Compiler::getExpressions(){
    if (expressions.size() == 0) {
        cout << "No expressions. Please call compile first" << endl;
    }

    return expressions;
}

/**
 * [Variable::getVariables gets variables that have been declared]
 * @return [the list of variables]
 */
vector<Variable> Compiler::getVariables(){
    if (variables.size() == 0) {
        cout << "No variables declared" << endl;
    }
    return variables;
}

/**
 * [Compiler::addVariable adds a new variable that has been declared]
 * @param var [the variable being declared]
 */
void Compiler::addVariable(Variable var){
    variables.push_back(var);
}

/**
 * [Compiler::compile calls scanFile and generates a vector of
 * expresions that will then be interpreted]
 * @param filename [name of file]
 */
void Compiler::compile(string filename){
    expressions = scanner.scanFile(filename);
}

int main(int argc, char const *argv[]) {
    Compiler compiler = Compiler();
    string filename = "test.c";

    compiler.compile(filename);

    return 0;
}
