//
// Created by eugene on 2019-03-25.
//

#include "ir.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

ir::ir(ParseTree parseTree, SymbolTable symbolTable)
:
    m_parseTree(parseTree)
,   m_symbolTable(symbolTable)
{
    vector<irInstruction> instructions;
    generateIR(parseTree);
    getGlobals(parseTree.root());
    int x;
}



// populate the IR from a  specified file
void ir::readFromFile(string filename)
{
    instructions.clear(); // remove the existing IR and replace it with the one from the file.
    ifstream in(filename);
    if (!in.is_open())
    {
        cout << "File " << filename << " could not be opened to read in IR" << endl;
    }
    else
    {
        for(string line; getline(in, line); )
        {
            istringstream ss(line);
            vector<string> params;

            do {
                string word;
                ss >> word;

                params.push_back(word);
            } while (ss);

            irInstruction temp = createIns(params);
            instructions.push_back(temp);
            params.clear();
        }
        in.close();
    }
}

// This function will write the current IR to a specified file.
void ir::writeToFile(string filename)
{
    ofstream out(filename);
    if (!out.is_open())
    {
        cout << "File " << filename << " could not be opened to write in IR" << endl;
    }
    else
    {
        cout << "Writing IR to file " << filename << endl;
        for (auto it : instructions)
        {
         out << (it.op == "" ? "" : it.op + " ") << (it.arg1 == "" ? "" : it.arg1 + " ") << (it.arg2 == "" ? "" : it.arg2 + " ") << (it.res == "" ? "" : it.res + " ") << endl;
        }
        out.close();
    }
}

// Used by the readFromFile function. This function will create a row that will be inserted into the IR table.
irInstruction ir::createIns(vector<string> params) {
    irInstruction instruction;
    string op = params.front();

    if (op == "JMP") // JUMP LABEL
    {
        instruction.op = op;
        instruction.res = params.at(1);
        instruction.arg1 = "";
        instruction.arg2 = "";
    } else if (op == "ADD")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "SUB")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "MUL")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "DIV")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "CMP")
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "JEQ")
    {
        instruction.op = "JEQ";
        instruction.res = "";
        instruction.arg1 = params.at(1); // this will be the result (1 or 0) from a CMP instruction stored in the table
        instruction.arg2 = params.at(2); // a label to jump to
    } else if (op == "JNEQ")
    {
        instruction.op = "JNEQ";
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2); // a label to jump to
    } else if (op == "LOAD") //LOAD ADDRESS REGISTER
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "STORE") //STORE ADDRESS REGISTER
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "POS") //+1
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "NEG") //-1
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "NOT") //!
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "EQ") //EQ VALUE VALUE RESULTREGISTER
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "NOTEQ") // NOTEQ VALUE VALUE RESULTREGISTER
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "LSTH")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "GRTH")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "LSEQ")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "GREQ")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    } else if (op == "GOTO")
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    } else if (op == "COPY")
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "FUNC")
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1); //function name
        instruction.arg2 = "";
    }
    else if (params.size() == 2 && op.at(op.size() - 1) == ':') // this is the case where we found a label
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = "";
        instruction.arg2 = "";
    }
    else
    {
        cout << "unknown instruction" << endl;
    }

    return instruction;
}

void ir::generateIR(ParseTree pTree) // will change this to an AST (?)
{
    pnode root = pTree.root();
    for (auto child : root.children())
    {
        this->getTreeChildren(child);
    }
}

void ir::getTreeChildren(pnode pn) {
    for (auto child : pn.children())
    {
        getTreeChildren(child);
    }
}

void ir::getGlobals(pnode root)
{
    vector<pnode> children = root.children();
    for (auto child : children)
    {
        if (child.rule() == "funcDecl")
        {
            m_globals.push_back(child);
            return;
        }
        getGlobals(child);
    }
}

void ir::readGlobals()
{
    for (auto global : m_globals)
    {
       if (global.rule() == "funcDecl")
       {
           // name of function is index 1 of child.
           // wrap this in a function.
           string funcName = global.children()[1].children()[0].rule();
           pnode paramNode = global.children()[1].children()[3];
           for (auto& param : paramNode.children())
           {
               // get symbols
               symbol* sym = m_symbolTable.lookup(param.rule());
           }
           pnode statementNode = global.children()[1].children()[5];
           for (auto& statement : statementNode.children())
           {

           }

       }
       else if (global.rule() == "varDecl")
       {

       }
    }
}
