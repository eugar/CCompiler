//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "cparser.hpp"

using namespace std;

Parser::Parser(vector<Token> &tokenList)
{
    m_tokenList = tokenList;
}

pair<StateTables::Action::ACTION, size_t> Parser::getAction(size_t currentState, string token)
{
    auto act = m_stateTable.m_action.table.find(currentState);
    return act->second.find(token)->second;
}

size_t Parser::getGoto(size_t currentState, string token)
{
    auto gt = m_stateTable.m_goto.table.find(currentState);
    return gt->second.find(token)->second;
}
