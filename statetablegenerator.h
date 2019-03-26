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
#include "statetables.h"

using namespace std;


class StateTableGenerator
{
public:

    StateTableGenerator();

private:

    /************************************************************************/
    /****************************** METHODS *********************************/
    /************************************************************************/

    // Clears the sets used by closure. These should be cleared before
    // every closure call.
    void clearTokenSets(){m_nonterminals.clear(); m_terminals.clear();}

    // Calculates the FIRST set.
    void first();
    // Calculates the FOLLOW set
    void follow();
    // Builds a basic set of LR1 Items. These are used for ease of
    // constructing LR1 Items during the canonical set creation.
    void buildLr1Sets();
    // This runs the Canonical Collection building algorithm. It directly calls
    // initialLr1Set() and generateLr1Closures().
    void createStates();
    // TODO: Implement createTables (partially implemented).
    // TODO: This should populate the action and goto tables.
    void createTables();
    // TODO: Implement writeTables().
    // TODO: This function should write the action and
    void writeTables();

    // Used by follow() to find follows that appear after non terminals
    // in a rule's right hand side.
    void findFollowRHS(string lhs, set<string> &rhsFollow);
    // Used by follow to place the appropriate tokens. Calls closure().
    void buildFollow(string lhs, set<string> &follow);
    // Called by various methods that require the closure of a symbol.
    void closure(string symbol);

    void generateLr1SetClosures();
    void initialLr1Set();

    /************************************************************************/
    /************************** MEMBER VARIABLES ****************************/
    /************************************************************************/

    // The grammar stored in the GrammarTree. This is a structure used
    // for organization and convenience. It is not a parse tree, but simply
    // a tree-like structure that links parent-child grammar rule relationships.
    GrammarTree m_grammar;

    map<string, set<string>> m_first;
    map<string, set<string>> m_follow;
    // Both m_terminals and m_nonterminals are sets used by the closure
    // algorithm. These sets are filled with all the terminals and
    // non terminal symbols reachable from the given token.
    set<const string*> m_terminals;
    set<const string*> m_nonterminals;

    // A collection of all possible LR1 Items.
    map<string, set<Lr1Item*>> m_lr1ItemIndex;
    set<Lr1Item, Lr1Compare> m_lr1ItemSet;

    // The canonical collection of LR1 Items.
    set<Lr1Item, Lr1Compare> m_lr1CC;
    // The action table
    StateTables m_tables;
    int m_nextState = 1;
    // Maps from a state to the set of LR1 Items with that state.
    map<size_t, set<Lr1Item, Lr1Compare>> m_ccCurSets;
    // Maps to an LR1 Item set by its previous state.
    map<size_t, set<Lr1Item, Lr1Compare>> m_ccPrevSets;

    /****************************************************************/
    /******************* INLINE HELPER FUNCTIONS ********************/
    /****************************************************************/

    bool inline contains(set<const string*> list, const string *token);
    bool inline contains(set<string> list, const string token);
    void inline vecCnstStrPtrToVecStr(const vector<const string*> &vecStrPtr, vector<string> &vecStr);
    void inline setStrToVecStr(const set<string> &setStr, vector<string> &vecStr);
    void inline gotoNextRule(vector<string>::iterator &it);
    void inline gotoNextRule(vector<const string*>::iterator &it);
    bool inline endOfRule(vector<string>::iterator &it);
    bool inline endOfRule(vector<const string*>::iterator &it);
    string inline findNextHandle(string rhs);
    string inline findNextHandle(string rhs, size_t phPos);
    vector<string> inline rhsToLr1ItemFormat(string lhsRule);
};

#endif //CCOMPILER_STATETABLE_GENERATOR_H
