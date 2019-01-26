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
