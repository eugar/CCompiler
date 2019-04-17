//
// Created by eugene on 2019-04-12.
//

#ifndef CCOMPILER_ASSEMBLYCONTEXT_H
#define CCOMPILER_ASSEMBLYCONTEXT_H

#include <string>
#include <vector>

using namespace std;

typedef struct reg {
    string name;
    int registerNum;
    int isUsed; // flag
    int age; // used for LRU
    int val; // might not be needed
} reg;

typedef struct regContext {
    int stackOffset;
    vector<reg> registerList;
    int numLabels;
} regContext;

class AssemblyContext {

public:
    AssemblyContext();

    void fillRegister(int tmpRegister, string name);
    int getRegister(string name);
    vector<reg> initRegList();
    void newScope();

    regContext context;


private:
};


#endif //CCOMPILER_ASSEMBLYCONTEXT_H
