//
// Created by Victor Gutierrez
//
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
    vector<Token> m_tokenList;
    // Currently using a third party state table generator, slightly modified to our purpose.
    //StateTableGenerator stateTableGenerator;
    Parser parser;
    ParseTree parseTree;
    SymbolTable symbolTable;

    int c;
    int pTokens = 0;
    int pTree = 0;
    int irRead = 0;
    int irWrite = 0;
    string irInFile;
    string irOutFile;


    if (argc < 2)
    {

HELP:
        cout << "ccompiler - compile a C file\n"
            << "Usage:\n"
            << argv[0] << " [FILE]\n"
            << "Optional arguments:\n"
            << "-h displays this help menu\n"
            << "-s displays list of tokens\n"
            << "-p displays parse tree\n"
            << "-i [file] populate the IR from a file\n"
            << "-o [file] write the IR to a file\n";
        return 1;
    }

    while ((c = getopt(argc, argv, "?:hspi:o:")) != -1)
    {
        switch (c)
        {
            case 's':
                pTokens = 1;
                break;
            case 'p':
                pTree = 1;
                break;
            case 'i':
                irRead = 1;
                irInFile = optarg;
                break;
            case 'o':
                irWrite = 1;
                irOutFile = optarg;
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
    parser.buildParseTree(parseTree, m_tokenList);
    parser.buildSymbolTable(symbolTable, parseTree.root(), "No type");

    if (pTokens)
    {
        Token::printTokens(m_tokenList);
    }

    if (pTree)
    {
        parseTree.printTree();
        cout << "--Symbol Table--" << endl;
        symbolTable.printRecords();
        cout << endl;
    }

    if (irRead) //todo: change these to handle the current programs IR
    {
        vector<irInstruction> test = ir::readFromFile(irInFile);
    }

    if (irWrite)
    {
        vector<irInstruction> test = ir::readFromFile("irexample.txt");
        ir::writeToFile(irOutFile, test);
    }

    return 0;
}
