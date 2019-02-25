//
// A header for global definitions
//
#ifndef CCOMPILER_DEFINITIONS_H
#define CCOMPILER_DEFINITIONS_H

#include <map>
#include <vector>
#include <string>
typedef struct LR1RHS
{
    std::string RHS;
    std::string lookahead;
    int set;
    int state;
}LR1RHS;
typedef std::map<std::string, std::vector<std::string>> ProductionRule;
typedef std::map<std::string, std::vector<std::string>*> ProductionRulePtr;
// character used as the placeholder in the closure/goto algorithm.
const static std::string c_phStr = "@";
const static char c_phCh = '@';

#endif //CCOMPILER_DEFINITIONS_H
