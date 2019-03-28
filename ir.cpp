//
// Created by eugene on 2019-03-25.
//

#include "ir.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

ir::ir()
{
    //vector<irInstruction> instructions;

}


vector<irInstruction> ir::readFromFile(string filename)
{
    vector<irInstruction> instructions;
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
    return instructions;
}

void ir::writeToFile(string filename, vector<irInstruction> instructions)
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

irInstruction ir::createIns(vector<string> params)
{
    irInstruction instruction;
    string op = params.front();

    if (op == "JMP") // JUMP LABEL
    {
        instruction.op = op;
        instruction.res = params.at(1);
        instruction.arg1 = "";
        instruction.arg2 = "";
    }
    else if (op == "ADD")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "SUB")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "MUL")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "DIV")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "CMP")
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "JEQ")
    {
        instruction.op = "JEQ";
        instruction.res = "";
        instruction.arg1 = params.at(1); // this will be the result (1 or 0) from a CMP instruction stored in the table
        instruction.arg2 = params.at(2); // a label to jump to
    }
    else if (op == "JNEQ")
    {
        instruction.op = "JNEQ";
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2); // a label to jump to
    }
    else if (op == "LOAD") //LOAD ADDRESS REGISTER
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "STORE") //STORE ADDRESS REGISTER
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "POS") //+1
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "NEG") //-1
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "NOT") //!
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "EQ") //EQ VALUE VALUE RESULTREGISTER
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "NOTEQ") // NOTEQ VALUE VALUE RESULTREGISTER
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "LSTH")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "GRTH")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "LSEQ")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "GREQ")
    {
        instruction.op = op;
        instruction.res = params.at(3);
        instruction.arg1 = params.at(1);
        instruction.arg2 = params.at(2);
    }
    else if (op == "GOTO")
    {
        instruction.op = op;
        instruction.res = "";
        instruction.arg1 = params.at(1);
        instruction.arg2 = "";
    }
    else if (op == "COPY")
    {
        instruction.op = op;
        instruction.res = params.at(2);
        instruction.arg1 = params.at(1);
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