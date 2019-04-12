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
    int assemblyFlag = 0;
    string irInFile;
    string irOutFile;
    string assemblyOutFile;


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
            << "-o [file] write the IR to a file\n"
            << "-w [file] write the assembly code out to a specified file. If this argument is not used, the compiler will automatically write to out.s" << endl;
        return 1;
    }

    while ((c = getopt(argc, argv, "?:hspi:o:w:")) != -1)
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
            case 'w':
                assemblyFlag = 1;
                assemblyOutFile = optarg;
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
    if(!parser.buildParseTree(parseTree, m_tokenList))
    {
        cout << "Unable to build parse tree\n";
        return -1;
    }
    parser.buildSymbolTable(symbolTable, parseTree.root());
    ir ir(parseTree, symbolTable);

    if (pTokens)
    {
        Token::printTokens(m_tokenList);
    }

    if (pTree)
    {
        parseTree.printTree();
        cout << "\n###### Symbol Table ######" << endl;
        symbolTable.printRecords(1);
        cout << endl;
    }

    if (irRead)
    {
        ir.readFromFile(irInFile);
    }

    if (irWrite)
    {
        //vector<irInstruction> test = ir::readFromFile("irexample.txt");
        ir.writeToFile(irOutFile);
    }

    Assembly a;

    if (assemblyFlag)
    { //todo: check to make sure the user-entered filename contains .s ??
        a = Assembly(assemblyOutFile);
    }
    else
    {
        a = Assembly();
    }

    a.generateCode(ir.instructions);
    a.closeOutput();

    return 0;
}
