#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "cscanner.hpp"
#include "cparser.hpp"
#include "token.h"
#include "symboltable.hpp"
#include <vector>
#include <fstream>

using namespace std;
namespace compiler
{
    void compile(string filename);

    void printTree(Parser &parser);
}
#endif // CCOMPILER_H
