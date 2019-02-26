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

    LR1RHS(std::string RHS, std::string lookAhead, int set = 0, int state = 0)
    {
        this->RHS = RHS;
        this->lookahead = lookAhead;
        this->set = set;
        this->state = state;
    }
    std::string RHS;
    std::string lookahead;
    int set;
    int state;

}LR1RHS;

struct RHSCompare
{
    bool operator() (const LR1RHS a, const LR1RHS b)
    {
        bool com = (a.RHS != b.RHS) || (a.lookahead != b.lookahead) || (a.set != b.set);
        return com;
    }
};
typedef std::map<std::string, std::vector<std::string>> ProductionRule;
typedef std::map<std::string, std::vector<std::string>*> ProductionRulePtr;
// character used as the placeholder in the closure/goto algorithm.
const static std::string c_phStr = "@";
const static char c_phCh = '@';

#endif //CCOMPILER_DEFINITIONS_H
