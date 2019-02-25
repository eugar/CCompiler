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
    void follow2();
    void findFollowLHS(string lhs, set<string> &follow);
    void findFollowRHS(string lhs, multimap<string, string> &rhsFollow);
    void buildFollow(string lhs, set<string> &follow);
    void closure(string symbol);
    void buildLRSet();
    void findLR0Keys(string token);
    void buildLR0Set();
/*
    void follow();
    void lookAhead(string state, string terminal, vector<string> &follow);
    void lookAhead(string state, vector<string> &firstTerminals);
    void backtrack(string lhs, string terminal);
    void findFollow(string lhs, string handle);
*/
    set<const string*> m_terminals;
    set<const string*> m_nonterminals;
    list<string> m_prevLHS;
    vector<string*> m_nonTerminalStack;
    vector<string> m_terminalStack;
    vector<string> m_branchNonTerminals;
    set<string> m_branched;
    string m_lhs;
    multimap<string, string> m_allLRItems;
    multimap<string, string> m_lr0Items;
    multimap<string, LR1RHS> m_lr1Items;
    set<string> m_lr0Stack;

    bool m_branching = false;

    /****************************************************************/
    /*******************INLINE HELPER FUNCTIONS *********************/
    /****************************************************************/

    // returns true if the list contains token.
    bool inline contains(set<const string*> list, const string *token);
    bool inline contains(set<string> list, const string token);
    void inline vecCnstStrPtrToVecStr(const vector<const string*> &vecStrPtr, vector<string> &vecStr);
    void inline setStrToVecStr(const set<string> &setStr, vector<string> &vecStr);
    void inline gotoNextRule(vector<string>::iterator &it);
    void inline gotoNextRule(vector<const string*>::iterator &it);
    bool inline endOfRule(vector<string>::iterator &it);
    bool inline endOfRule(vector<const string*>::iterator &it);
};

#endif //CCOMPILER_STATETABLE_GENERATOR_H
