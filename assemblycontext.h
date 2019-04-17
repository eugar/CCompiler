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
    int val;
} reg;

typedef struct registerList {
    size_t size;
    vector<reg> registers;
} registerList;

typedef struct regContext {
    int stackOffset;
    registerList list;
    int numLabels;
} regContext;

class AssemblyContext {

public:
    AssemblyContext();

    void fillRegister(string name, int offset);
    int getRegister(string name);
    void newScope();

    regContext context;


private:
};


#endif //CCOMPILER_ASSEMBLYCONTEXT_H
