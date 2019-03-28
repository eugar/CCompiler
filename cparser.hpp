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

typedef pair<StateTables::Action::ACTION, size_t> act;

class Parser
{
public:
    Parser();
    void printParseTree();
    void buildParseTree(ParseTree &parseTree, vector<Token> &tokenList);

private:
    size_t runAction(act actRun, ParseTree &parseTree, Token token);
    void reduce(ParseTree &parseTree);

    // Gets the action from current state and rule.
    inline act getAction(size_t currentState, string rule);
    // Gets the goto state from current state and rule.
    inline size_t getGoto(size_t currentState, string rule);
    inline void replaceNodes(ParseTree &parseTree);

    size_t m_curState;
    StateTables m_stateTable;
    vector<string> m_stateStack;
    act m_action;
    pnode m_newRoot;
    vector<pnode> m_newNodes;
    // <  RHS ,   LHS >
    map<string, string> m_grammarRed;

};

#endif //CCOMPILER_CPARSER_H
