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
    vector<vector<int>> m_setTable;

    void clearTokenSets(){m_nonterminals.clear(); m_terminals.clear();}

    void first();
    void follow2();
    void findFollowRHS(string lhs, set<string> &rhsFollow);
    void buildFollow(string lhs, set<string> &follow);
    void closure(string symbol);
    void buildLRSet();
    void findLR0Keys(string token);
    void buildLR0Set();
    void buildInitLR1Set();
    void buildLR1Sets();

    void createTable();
    void createSets();
    void createSets(set<string> &terminal, string &handle, string &lhs);

    set<const string*> m_terminals;
    set<const string*> m_nonterminals;
    string m_lhs;
    map<string, set<string>> m_allLRItems;
    map<string, set<string>> m_lr0Items;
    map<string, set<LR1RHS, RHSCompare>> m_lr1Items;
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
    string inline findNextHandle(string rhs);
    bool inline advancePH(string &rhs);
    bool inline advancePHSets(string &rhs);
};

#endif //CCOMPILER_STATETABLE_GENERATOR_H
