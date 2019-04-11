//
// Created by Victor Gutierrez
//
#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "cscanner.hpp"
#include "token.h"
#include "symboltable.hpp"
#include "cparser.hpp"
#include "ir.h"
#include "assembly.h"
#include <vector>
#include <fstream>

using namespace std;
namespace compiler
{
    void compile(string filename);
}
#endif // CCOMPILER_H
