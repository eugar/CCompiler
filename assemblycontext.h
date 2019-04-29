//
// Created by eugene on 2019-04-12.
//

#ifndef CCOMPILER_ASSEMBLYCONTEXT_H
#define CCOMPILER_ASSEMBLYCONTEXT_H

#include <string>
#include <vector>

using namespace std;

typedef struct variableT {
    string name;
    int offset;
} variableT;

typedef struct stackT {
    int stackOffset;
    vector<variableT> variables;
    int numLabels;
} stackT;

class AssemblyContext {

public:
    AssemblyContext();

    int getOffset(string name);
    void setOffset(string name, int offset);
    void newScope();

    stackT context;


private:
};


#endif //CCOMPILER_ASSEMBLYCONTEXT_H
