#include "ccompiler.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

/**
 * [Compiler::Compiler Compiler contructor]
 */
Compiler::Compiler(){
    scanner = Scanner();
    expressions = vector<Expression>();
}

/**
 * [Compiler::getExpressions gets the expressions generated
 * by the scanner to eventually get interpreted]
 * @return [a vector of expressions]
 */
vector<Expression> &Compiler::getExpressions(){
    if (expressions.size() == 0) {
        cout << "No expressions. Please call compile first" << endl;
    }

    return expressions;
}

/**
 * [Variable::getVariables gets variables that have been declared]
 * @return [the list of variables]
 */
vector<Variable> &Compiler::getVariables(){
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

    // populate tokenList
    scanner.scanFile(filename, tokenList);
    // parse tokens
    // do more stuff
}

void Compiler::printTokens(){ 
    for (auto it = tokenList.begin(); it != tokenList.end(); it++){
        scanner.printTokens(it->first, it->second);
    }
}

int main(int argc, char * const argv[]) {
    int c;
    int printTokens = 0;
    int printTree = 0;

    Compiler compiler = Compiler();

    if (argc < 2) {
HELP:
        cout << "ccompiler - compile a C file\n"
            << "Usage:\n"
            << argv[0] << " [FILE]\n"
            << "Optional arguments:\n"
            << "-h displays this help menu\n"
            << "-s displays list of tokens\n"
            << "-p displays parse tree\n";
        return 1;
    }

    while ((c = getopt(argc, argv, "?:hsp")) != -1) {
        switch (c) {
            case 's':
                printTokens = 1;
                break;
            case 'p':
                printTree = 1;
            case 'h':
                goto HELP;
        }
    }

    if (optind != argc-1) {
        cout << "Invalid number of arguments.\n";
        return 1;
    }

    string filename = argv[optind];


    compiler.compile(filename);

    if (printTokens) {
        compiler.printTokens();
    }

    if (printTree) {
        //print tree
    }

    return 0;
}
