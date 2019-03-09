#include "ccompiler.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "statetablegenerator.h"
#include <stdlib.h>


using namespace std;

// This does not need to be a class. Changed this to be the
// main logic. This file should include others, but the
// the others should not include each other (where it can be
// avoided) or this file.
using namespace compiler;

int main(int argc, char * const argv[])
{
    Scanner m_scanner;
    // eventually I will move grammar building to another class
    // for now it builds in the constructor of Parser
//    Parser m_parser;
    vector<Token> m_tokenList;

    StateTableGenerator stateTableGenerator;


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

    m_scanner.openFile(filename, m_tokenList);


    if (pTokens)
    {
        Token::printTokens(m_tokenList);
    }

    if (pTree)
    {
//        printTree(m_parser);
    }


    return 0;
}


void symbolTestFunction()
{
    SymbolTable st = SymbolTable();
    int data = 3;

    st.insert("var", "string", &data);
}
