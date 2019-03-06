//
// grammar related functions
//
#include "definitions.h"

#ifndef CCOMPILER_GRAMMAR_H
#define CCOMPILER_GRAMMAR_H

namespace grammar
{
void reader(std::vector<std::string> &pRuleIndex,
        ProductionRule &pRule,
        const std::string& filename = "expr.txt");

bool nextRule(std::string line, int &start, int &length);
}

#endif //CCOMPILER_GRAMMAR_H
