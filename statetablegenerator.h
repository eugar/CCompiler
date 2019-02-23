//
// Created by mmalett on 2/22/19.
//

#ifndef CCOMPILER_STATETABLE_GENERATOR_H
#define CCOMPILER_STATETABLE_GENERATOR_H

#include <string>
#include <vector>
#include <map>
#include "grammartree.h"
#include "definitions.h"

using namespace std;

class StateTableGenerator
{
public:

    StateTableGenerator();


private:

    GrammarTree m_grammar;
    map<string, set<string>> m_first;
    map<string, set<string>> m_follow;

    void clearTokenSets(){m_nonterminals.clear(); m_terminals.clear();}

    void first();
    void follow();
    void closure(string symbol);
    void lookAhead(string state);


    // returns true if the list contains token.
    bool contains(set<const string*> list, const string *token);


    set<const string*> m_terminals;
    set<const string*> m_nonterminals;
    vector<string> m_prevLHS;
};

#endif //CCOMPILER_STATETABLE_GENERATOR_H
