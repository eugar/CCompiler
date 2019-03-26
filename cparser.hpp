//
// created by Victor Gutierrez on Mar 25, 2019
//
#ifndef CCOMPILER_CPARSER_H
#define CCOMPILER_CPARSER_H

#include "statetables.h"
#include <vector>

using namespace std;

class Parser
{
public:
    Parser();
    void printParseTree();

private:
    pair<StateTables::Action::ACTION, size_t> getAction(size_t currentState, string token);
    size_t getGoto(size_t currentState, string token);
    void setState(size_t newState){m_currentState = newState;}

    size_t m_currentState;
    StateTables m_stateTable;
    vector<string> m_stateStack;
};

#endif //CCOMPILER_CPARSER_H
