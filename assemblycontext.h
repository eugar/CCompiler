//
// Created by eugene on 2019-04-12.
//

#ifndef CCOMPILER_ASSEMBLYCONTEXT_H
#define CCOMPILER_ASSEMBLYCONTEXT_H

#include <string>
#include <vector>

using namespace std;

typedef struct item {
    string name;
    int offset;
} item;

typedef struct stackItems {
    size_t size;
    vector<item> items;
} stackItems;

typedef struct itemContext {
    int stackOffset;
    stackItems stack;
    int numLabels;
} itemContext;

class AssemblyContext {

public:
    AssemblyContext();

    stackItems newStack();
    void setOffset(string name, int offset, stackItems stack);
    int getOffset(string name, stackItems stack);
    void newScope();

    itemContext context;


private:
};


#endif //CCOMPILER_ASSEMBLYCONTEXT_H
