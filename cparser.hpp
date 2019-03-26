//
// created by Victor Gutierrez on Mar 25, 2019
//
#ifndef CCOMPILER_CPARSER_H
#define CCOMPILER_CPARSER_H

#include "statetables.h"
#include "parsetree.hpp"
#include "token.h"
#include <vector>

using namespace std;

class Parser
{
public:
    Parser(vector<Token> &tokenList);
    void printParseTree();

private:
    // Gets the action from current state and token.
    pair<StateTables::Action::ACTION, size_t> getAction(size_t currentState, string token);
    // Gets the goto state from current state and token.
    size_t getGoto(size_t currentState, string token);
    void setState(size_t newState){m_currentState = newState;}

    size_t m_currentState;
    StateTables m_stateTable;
    vector<string> m_stateStack;
    vector<Token> m_tokenList;
};

#endif //CCOMPILER_CPARSER_H
