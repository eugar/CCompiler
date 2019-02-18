#include "ccompiler.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "grammartree.h"

using namespace std;

// This does not need to be a class. Change this to be the
// main logic. This file should include all the others, but
// the others should not include each other or this file.
using namespace compiler;

int main(int argc, char * const argv[])
{
    Scanner m_scanner;
    // eventually I will move grammar building to another class
    // for now it builds in the constructor of Parser
    Parser m_parser;
    vector<Token> m_tokenList;
    vector<Expression> m_expressionList;
    vector<Variable> m_variableList;

    int c;
    int pTokens = 0;
    int pTree = 0;

    if (argc < 2)
    {
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

    while ((c = getopt(argc, argv, "?:hsp")) != -1)
    {
        switch (c)
        {
            case 's':
                pTokens = 1;
                break;
            case 'p':
                pTree = 1;
                break;
            case 'h':
                goto HELP;
        }
    }

    if (optind != argc-1)
    {
        cout << "Invalid number of arguments.\n";
        return 1;
    }

    string filename = argv[optind];
    
    m_scanner.scanFile(filename, m_tokenList);


    if (pTokens)
    {
        printTokens(m_tokenList, m_scanner);
    }

    if (pTree)
    {
        printTree(m_parser);
    }


    return 0;
}


// Prints the list of tokens.
void compiler::printTokens(vector<Token> &tokenList, Scanner &scanner)
{
    for (auto it = tokenList.begin(); it != tokenList.end(); it++)
    {
        scanner.printTokens(it->first, it->second);
    }
}

void compiler::printTree(Parser &parser)
{
    parser.printParseTree();
}


/*
// Gets the expressions generated by the scanner to eventually
// be interpreted. Returns the list of expressions.
vector<Expression> &getExpressions()
{
//    if (m_expressionList.size() == 0)
//    {
//        cout << "No expressions. Please call compile first" << endl;
//    }
//
//    return m_expressionList;
}
*/
/**
 * [Variable::getVariables gets variables that have been declared]
 * @return [the list of variables]
 */
/*
// Gets variables that have been declared. Returns the
// list of variables.
vector<Variable> &getVariables()
{
//    if (m_variableList.size() == 0)
//    {
//        cout << "No variables declared" << endl;
//    }
//    return m_variableList;
}
*/
/**
 * [addVariable adds a new variable that has been declared]
 * @param var [the variable being declared]
 */
 /*
// Adds a new variable to the variable list.
// Returns the new variable.
//void addVariable(Variable var) // The new variable
//{
//    m_variableList.push_back(var);
//}
*/
/* Calls scanFile and generates expressions
//void compile(string filename) // Path of the file
{

    // populate tokenList
    m_scanner.scanFile(filename, m_tokenList);
    // parse tokens
    m_parser.parseTokens(m_tokenList);
    // do more stuff
}
*/

