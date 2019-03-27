//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "cparser.hpp"

using namespace std;

Parser::Parser()
{
    m_curState = 0;
    m_stateStack.push_back("0");
    newRoot = nullptr;
}

void Parser::buildParseTree(ParseTree &parseTree, vector<Token> &m_tokenList)
{
    for(auto token : m_tokenList)
    {
        m_action = getAction(m_curState, token.getLiteral());
        m_curState = runAction(m_action, parseTree);
    }
}

// TODO: Fix the reduce and accept cases
//       add a reject case
size_t Parser::runAction(act actRun, ParseTree &parseTree)
{
    if (actRun.first == StateTables::Action::ACTION::SHIFT)
    {
        //cout<< "shift: " << actRun.second << endl;
        return actRun.second;
    }
    else if(actRun.first == StateTables::Action::ACTION::REDUCE)
    {
        // reduce the parse tree
        //cout<< "reduce: " << actRun.second << endl;

        if (newRoot != nullptr && newNodes.size() < 0) {
            parseTree.reduce(newRoot, newNodes);
        }
    }
    else if(actRun.first == StateTables::Action::ACTION::ACCEPT)
    {
        // End the parse building
        //cout << "accepted!" << endl;
    }
    else
    {
        // reject state
        // End the parse building
        // throw an error
    }
    return 0;
}

act Parser::getAction(size_t currentState, string token)
{
    auto a = m_stateTable.m_action.table.find(currentState);
    return a->second.find(token)->second;
}

size_t Parser::getGoto(size_t currentState, string token)
{
    auto gt = m_stateTable.m_goto.table.find(currentState);
    return gt->second.find(token)->second;
}
