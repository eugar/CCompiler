//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "cparser.hpp"
#include "token.h"
#include <cstdio>
#include <fstream>
#include <regex>

using namespace std;



void Parser::hardcodeTest()
{
    m_stateTable.m_action.insert(0, 3, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(1, 0, "$", StateTables::Action::ACTION::ACCEPT);
    m_stateTable.m_action.insert(1, 3, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(2, 3, "$", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(2, 3, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(3, 6, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(3, 7, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(4, 2, "$", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(4, 2, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(5, 8, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(6, 10, ")", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(6, 6, "(", StateTables::Action::ACTION::SHIFT);
    m_stateTable.m_action.insert(7, 5, "$", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(7, 5, "(", StateTables::Action::ACTION::REDUCE);
    m_stateTable.m_action.insert(8, 4, "$", StateTables::Action::ACTION::REDUCE);
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
    //hardcodeTest();
    loadTables();
    // for breakpoint
    int i;
}

void Parser::buildParseTree(ParseTree &parseTree, vector<Token> &tokenList)
{
    int i = 0;
    tokenList.push_back(Token("$", ERR, 0));
    convertTokenList(tokenList);

    while(m_tokenStack.size() >= 1)
    {

        //if (m_tokenStack.begin()->getType() == ID) {
        //    cout << "ID: " << m_tokenStack.begin()->rule() << endl;
        //    m_action = getAction("ID");
        //}
        //else{
            m_action = getAction(m_tokenStack.begin()->rule());
        //}

        if (m_stateStack.back() == -1) {
            cout << "error: -1" << endl;
            break;
        }

        if(!runAction(m_action, parseTree, m_tokenStack[0]))
        {
            break;
        }
        cout << "step "<< i ;
        cout << "\t\tstate: " << m_stateStack.back();
        cout << "\tInput: " << m_tokenStack.begin()->rule() << endl;
        printStack();
        cout << "\n";
        i++;
    }
    parseTree.newRoot(m_newRoot);
}

void Parser::loadTables(string path)
{
    string line;
    regex rx("\\s+");
    ifstream fs(path);
    int section = 0;
    while (getline(fs, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        else
        {
            // read rules
            if (section == 0)
            {
                if (line[0] == '~')
                {
                    section++;
                }
                else
                {
                    sregex_token_iterator it(line.begin(), line.end(), rx, -1);
                    sregex_token_iterator end;
                    string lhs = it->str();
                    it++;
                    string rhs;
                    while (it != end)
                    {
                        rhs.append(it->str() + " ");
                        it++;
                    }
                    rhs.erase(rhs.end() - 1);
                    m_grammarRed[rhs] = lhs;
                }
            }
            else
            {
                sregex_token_iterator it(line.begin(), line.end(), rx, -1);
                unsigned long state = stoul(it->str());
                it++;
                string token = it->str();
                it++;
                // goto entry
                if (isdigit(it->str()[0]))
                {
                    m_stateTable.m_goto.insert(state, stoul(it->str()), token);
                }
                    // action entry
                else
                {
                    StateTables::Action::ACTION action;
                    string a = it->str().substr(0,1);
                    if (a == "r")
                    {
                        if (it->str() == "r0")
                        {
                            action = StateTables::Action::ACTION::ACCEPT;
                        }
                        else
                        {
                            action = StateTables::Action::ACTION::REDUCE;
                        }
                    }
                    else
                    {
                        action = StateTables::Action::ACTION::SHIFT;
                    }
                    int newState = stoi(it->str().substr(1));
                    m_stateTable.m_action.insert(state, newState, token, action);

                }
            }
        }
    }
}

bool Parser::runAction(act actRun, ParseTree &parseTree, pnode rule)
{
    if (actRun.first == StateTables::Action::ACTION::SHIFT)
    {
        //pnode newpnode = pnode(rule);
        m_nodeStack.push_back(rule);
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
    string temp = "";
    vector<pnode> tempNodes;
    int i = m_nodeStack.size()-1;
    int x = i;

    if (m_nodeStack.size() == 1) {
        try
        {
            m_newRoot = m_grammarRed.at(m_nodeStack[0].rule());
            m_newRoot.addChild(m_nodeStack[0]);
            //cout << "reducing: " << m_nodeStack[0].rule() << " to: " << m_newRoot.rule() << endl;
            replaceStack(parseTree, 0);
            return;
        }
        catch(const std::out_of_range& oor)
        {
            return;
        }
    }

    while(i >= 0)
    {
        if (temp == "")
        {
            temp = m_nodeStack[i].rule();
        }
        else
        {
            temp = m_nodeStack[i].rule() + " " + temp;
        }

        try
        {
            m_newRoot = m_grammarRed.at(temp);
            x = i;
            //cout << "reducing: " << temp << " to: " << m_newRoot.rule() << endl;
        }
        catch(const std::out_of_range& oor)
        {
            //cerr << "\"" << temp << "\" - could not be reduced" << endl;
        }
        i--;
    }
    for(int y = m_nodeStack.size()-1; y >= x; y--)
    {
        tempNodes.insert(tempNodes.begin(), m_nodeStack[y]);
    }
    for(auto child : tempNodes)
    {
        m_newRoot.addChild(child);
    }
    replaceStack(parseTree, x);
}

void Parser::convertTokenList(vector<Token> tokenList)
{
    for(auto tok : tokenList)
    {
        pnode node;
        pnode child;
        switch (tok.getType()) {
            case ID:
                child.setRule(tok.getLiteral());
                node.setRule("ID");
                node.addChild(child);
                m_tokenStack.push_back(node);
            break;
            case NUMCONST:
                child.setRule(tok.getLiteral());
                node.setRule("NUMCONST");
                node.addChild(child);
                m_tokenStack.push_back(node);
            break;
            case CHAR:
                child.setRule(tok.getLiteral());
                node.setRule("CHARCONST");
                node.addChild(child);
                m_tokenStack.push_back(node);
            break;
            default:
                node.setRule(tok.getLiteral());
                m_tokenStack.push_back(node);
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

    //cout << "goto: " << m_newRoot.rule() << ": " << m_stateStack.back() << endl;
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
