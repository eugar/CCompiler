//
// Created by eugene on 2019-03-25.
//

#ifndef CCOMPILER_IR_H
#define CCOMPILER_IR_H

#include <string>
#include <vector>

using namespace std;

typedef struct
{
    string op;
    string arg1;
    string arg2;
    string res;
} irInstruction;

class ir {

public:
    ir();
    static vector<irInstruction> readFromFile(string filename);
    static void writeToFile(string filename, vector<irInstruction> instructions);

private:
    static irInstruction createIns(vector<string> params);

};


#endif //CCOMPILER_IR_H
