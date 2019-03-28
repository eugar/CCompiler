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
    //m_newRoot = nullptr;
}

void Parser::buildParseTree(ParseTree &parseTree, vector<Token> &m_tokenList)
{
    for(auto token : m_tokenList)
    {
        m_action = getAction(m_curState, token.getLiteral());
        m_curState = runAction(m_action, parseTree, token);
    }
}

// TODO: Fix the reduce and accept cases
//       add a reject case
size_t Parser::runAction(act actRun, ParseTree &parseTree, Token token)
{
    if (actRun.first == StateTables::Action::ACTION::SHIFT)
    {
        pnode newpnode = pnode(token.getLiteral());
        m_newNodes.push_back(newpnode);
        //cout<< newpnode.rule() << " shift: " << actRun.second << endl;
        return actRun.second;
    }
    else if(actRun.first == StateTables::Action::ACTION::REDUCE)
    {
        // reduce the parse tree
        //cout<< "reduce: " << actRun.second << endl;

        if (m_newNodes.size() < 0) {
            reduce(parseTree);
        }
        replaceNodes(parseTree);
    }
    else if(actRun.first == StateTables::Action::ACTION::ACCEPT)
    {
        // End the parse building
        cout << "accepted!" << endl;
    }
    else
    {
        // reject state
        // End the parse building
        // throw an error
    }
    return 0;
}

void Parser::reduce(ParseTree &parseTree)
{
    string temp = "";
    for(auto node : m_newNodes)
    {
        temp += " " + node.rule();
    }
    try
    {
        m_newRoot = m_grammarRed.at(temp);
        parseTree.reduce(m_newRoot, m_newNodes);
    }
    catch(const std::out_of_range& oor)
    {
        cerr << temp << "could not be reduced: " << endl;
    }
}

act Parser::getAction(size_t currentState, string rule)
{
    auto a = m_stateTable.m_action.table.find(currentState);
    return a->second.find(rule)->second;
}

size_t Parser::getGoto(size_t currentState, string rule)
{
    auto gt = m_stateTable.m_goto.table.find(currentState);
    return gt->second.find(rule)->second;
}

void Parser::replaceNodes(ParseTree &parseTree)
{
    m_newNodes.clear();
    m_newNodes = parseTree.rootChildren();
}
