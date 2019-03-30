//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "cparser.hpp"
#include "token.h"
#include <cstdio>

using namespace std;

void Parser::hardcodeTest()
{
    m_stateTable.m_action.insert(0, 3, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(1, 0, "%", StateTables::Action::ACTION::ACCEPT);
    m_stateTable.m_action.insert(1, 3, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(2, 3, "%", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(2, 3, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(3, 6, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(3, 7, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(4, 2, "%", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(4, 2, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(5, 8, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(6, 10, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(6, 6, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(7, 5, "%", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(7, 5, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(8, 4, "%", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(8, 4, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(9, 11, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(10, 5, ")", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(11, 4, ")", StateTables::Action::ACTION::REDUCE);

    m_stateTable.m_goto.insert(0, 1, "List");
    m_stateTable.m_goto.insert(0, 2, "Pair");
    m_stateTable.m_goto.insert(1, 4, "Pair");
    m_stateTable.m_goto.insert(3, 5, "Pair");
    m_stateTable.m_goto.insert(6, 9, "Pair");

    m_grammarRed["List"] = "Goal";
    m_grammarRed["List Pair"] = "List";
    m_grammarRed["Pair"] = "List";
    m_grammarRed["( Pair )"] = "Pair";
    m_grammarRed["( )"] = "Pair";
}

Parser::Parser()
{
    m_stateStack.push_back(0);
    hardcodeTest();
}

void Parser::buildParseTree(ParseTree &parseTree, vector<Token> &tokenList)
{
    int i = 0;
    tokenList.push_back(Token("%", ERR, 0));
    m_tokenStack = tokenList;

    while(m_tokenStack.size() >= 1)
    {
        m_action = getAction(m_tokenStack.begin()->getLiteral());

        if (m_stateStack.back() == -1) {
            cout << "error: -1" << endl;
            break;
        }

        if(!runAction(m_action, parseTree, m_tokenStack.begin()->getLiteral()))
        {
            break;
        }
        cout << "step "<< i ;
        cout << "\t\tstate: " << m_stateStack.back();
        cout << "\tInput: " << m_tokenStack.begin()->getLiteral() << endl;
        printStack();
        cout << "\n";
        i++;
    }
    parseTree.newRoot(m_newRoot);
}

bool Parser::runAction(act actRun, ParseTree &parseTree, string rule)
{
    if (actRun.first == StateTables::Action::ACTION::SHIFT)
    {
        pnode newpnode = pnode(rule);
        m_nodeStack.push_back(newpnode);
        cout<< "\nshift: " << actRun.second << endl;
        m_stateStack.push_back(actRun.second);
        m_tokenStack.erase(m_tokenStack.begin());
        return true;
    }
    else if(actRun.first == StateTables::Action::ACTION::REDUCE)
    {
        // reduce the parse tree
        reduce(parseTree);
        return true;
    }
    else if(actRun.first == StateTables::Action::ACTION::ACCEPT)
    {
        // End the parse building
        cout << "accepted!" << endl;
        m_tokenStack.erase(m_tokenStack.begin());
        return false;
    }
    else
    {
        cout << "reject"<< endl;
        // reject state
        // End the parse building
        // throw an error
        m_tokenStack.erase(m_tokenStack.begin());
        return false;
    }
}

void Parser::reduce(ParseTree &parseTree)
{
    string temp = m_nodeStack[m_nodeStack.size()-1].rule();
    vector<pnode> tempNodes = {m_nodeStack[m_nodeStack.size()-1]};

    if (m_nodeStack.size() == 1) {
        try
        {
            m_newRoot = m_grammarRed.at(m_nodeStack[0].rule());
            m_newRoot.addChild(m_nodeStack[0]);
            cout << "reducing: " << m_nodeStack[0].rule() << " to: " << m_newRoot.rule() << endl;
            replaceStack(parseTree, 0);
            return;
        }
        catch(const std::out_of_range& oor)
        {
            return;
        }
    }

    for(int i = m_nodeStack.size()-2; i >= 0; i--)
    {
        temp = m_nodeStack[i].rule() + " " + temp;
        tempNodes.insert(tempNodes.begin(), m_nodeStack[i]);
        //tempNodes.push_back(m_nodeStack[i]);
        //printStack();
        try
        {
            m_newRoot = m_grammarRed.at(temp);
            for(auto child : tempNodes)
            {
                m_newRoot.addChild(child);
            }
            cout << "reducing: " << temp << " to: " << m_newRoot.rule() << endl;
            replaceStack(parseTree, i);
            break;
        }
        catch(const std::out_of_range& oor)
        {
            //cerr << "\"" << temp << "\" - could not be reduced" << endl;
        }
    }
}

act Parser::getAction(string rule)
{
    auto a = m_stateTable.m_action.table.find(m_stateStack.back());
    if (a == m_stateTable.m_action.table.end()) {
        cout << "State: " << m_stateStack.back() << " not found\n";
        //return make_pair<StateTables::Action::SHIFT, -1>;
    }
    auto am = a->second.find(rule);
    if (am == a->second.end()) {
        cout << "Action - " <<rule<<" : "<<m_stateStack.back() <<" not found\n";
        //auto ret = make_pair<StateTables::Action::SHIFT, -1>;
        //return ret;
    }
    return am->second;
}

size_t Parser::getGoto(string rule)
{
    auto gt = m_stateTable.m_goto.table.find(m_stateStack.back());
    if (gt == m_stateTable.m_goto.table.end()) {
        cout << "State: " << m_stateStack.back() << " not found\n";
        return -1;
    }
    auto r = gt->second.find(rule);
    if (r == gt->second.end()) {
        cout << "Rule: " << rule << " not found\n";
        return -1;
    }
    return r->second;
}

void Parser::replaceStack(ParseTree &parseTree, int i)
{
    for(int j = m_nodeStack.size()-1; j >= i; j--)
    {
        m_nodeStack.pop_back();
        m_stateStack.pop_back();
    }

    m_nodeStack.push_back(m_newRoot);
    m_stateStack.push_back(getGoto(m_newRoot.rule()));

    cout << "goto: " << m_newRoot.rule() << ": " << m_stateStack.back() << endl;
}

void Parser::printStack()
{
    cout << "stack - ";
    int i = 0;
    for(auto n : m_nodeStack)
    {
        cout << m_stateStack[i] << " " << n.rule() << " ";
        i++;
    }
    if (i == m_stateStack.size()-1) {
        cout << m_stateStack[i];
    }
    cout << endl;
}
