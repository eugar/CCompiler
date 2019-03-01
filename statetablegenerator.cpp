//
// Created by mmalett on 2/22/19.
//
#include <iostream>
#include <algorithm>
#include "statetablegenerator.h"

using namespace std;

// StateTableGenerator constructor
StateTableGenerator::StateTableGenerator()
{
    first();
    follow2();
    buildLRSet();
    findLR0Keys((m_grammar.ruleIndex())[0]);
    buildLR0Set();
    buildInitLR1Set();

    int w;

}

void StateTableGenerator::createTable()
{
    size_t prevSize = 0;
    do
    {
        prevSize = m_lr1Items.size();
        createSets();

    } while(m_lr1Items.size() > prevSize);
}
void StateTableGenerator::createSets()
{
    for (auto lr1Map : m_lr1Items)
    {
        for (auto rhs : lr1Map.second)
        {
            advancePHSets(rhs.RHS);
            string s = findNextHandle(rhs.RHS);
            auto terminals = m_first.find(s);
            // If s is not a terminal.
            string lhs = *(const_cast<string*>(&lr1Map.first));
            if (terminals != m_first.end())
            {
                createSets(terminals->second, s, lhs);
            }
        }
    }
}
void StateTableGenerator::createSets(set<string> &terminals, string &handle, string &lhs)
{
    for (auto t : terminals)
    {
        auto follows = m_follow.find(handle);
        if (follows != m_follow.end())
        {
            for (auto f : follows->second)
            {
                auto lr1Rhs = m_lr1Items.at(lhs);
                auto lr1 = *(lr1Rhs.begin());
                // we want to replace the string after the placeholder
                // with the terminals that come next.
                m_lr1Items[lhs].emplace(LR1RHS(lr1.RHS, f));
            }
        }
    }
}

// Builds the rest of the LR1 sets after buildInit
void StateTableGenerator::buildLR1Sets()
{

}
// Given a string, it returns a list of all valid terminals that can be seen
// Loops over 'first()' for every LHS rule.
void StateTableGenerator::first()
{
    for (auto &lhs : m_grammar.ruleIndex())
    {
        closure(lhs);
        for (auto terminal : m_terminals)
        {
            m_first[lhs].emplace(*terminal);
        }
        clearTokenSets();
    }
}

// Creates a follow set for every
// LHS element in the grammar
void StateTableGenerator::follow2()
{
    // loop over all left hand side handles.
    for (auto lhs = m_grammar.ruleIndex().begin(); lhs != m_grammar.ruleIndex().end(); lhs++)
    {
        set<string> follow;
        findFollowRHS(*lhs, follow);
        buildFollow(*lhs, follow);
    }
}

// Calls closure on every follow of every LHS element
void StateTableGenerator::buildFollow(string lhs, set<string> &follow)
{
    for (auto token : follow)
    {
        if (isupper(token[0]))
        {
            m_follow[lhs].emplace(token);
        }
        else
        {
            closure(token);
            for (auto terminal : m_terminals)
            {
                m_follow[lhs].emplace(*terminal);
            }
            clearTokenSets();
        }
    }
}

// loops over all LHSs and searches the RHSs for instances of each rule.
void StateTableGenerator::findFollowRHS(string nonterminal, set<string> &rhsFollow)
{
    for (auto& lhs : m_grammar.ruleIndex())
    {
        vector<const string*> rules = m_grammar.rule(lhs)->rules();
        for (auto rule = rules.begin(); rule != rules.end(); rule++)
        {
            if (nonterminal == **rule)
            {
                rule++;
            }
            else
            {
                continue;
            }
            if (!endOfRule(rule))
            {
                rhsFollow.insert(**rule);
            }

        }
    }
    return;
}

// builds strings with the place holder at the beginning
void StateTableGenerator::buildLRSet()
{
    for (auto& lhs : m_grammar.ruleIndex())
    {
        auto rules = m_grammar.rule(lhs)->rules();
        string rule;
        for (auto pRule = rules.begin(); pRule != rules.end(); pRule++)
        {
            while (!endOfRule(pRule) && (*pRule)->compare("EOF") != 0)
            {
                rule += **pRule + " ";
                pRule++;
            }
            if (!rule.empty())
            {
                // delete the last space
                rule.erase(rule.size() -1, 1);
                //insert PH at start
                rule.insert(0, 1, c_phCh);
                // add to lr0 items
                m_allLRItems[lhs].emplace(rule);
                rule.clear();
            }
            gotoNextRule(pRule);
        }
    }
}

void StateTableGenerator::findLR0Keys(string token)
{
    if (token == m_grammar.ruleIndex()[0])
    {
        m_lr0Stack.emplace(token);
    }
    auto rules = m_grammar.rule(token)->rules();
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        if (**rule == token)
        {
            gotoNextRule(rule);
        }
        if (isupper((**rule)[0]))
        {
            gotoNextRule(rule);
        }
        if (endOfRule(rule))
        {
            continue;
        }
        m_lr0Stack.emplace(**rule);
        findLR0Keys(**rule);
        gotoNextRule(rule);
    }
}

void StateTableGenerator::buildLR0Set()
{
    for (auto& lhs : m_lr0Stack)
    {
        for(auto rule : m_allLRItems[lhs])
        {
            m_lr0Items[lhs].emplace(rule);
        }
        cout << endl;
    }
}

// Builds the initial LR1 sets
void StateTableGenerator::buildInitLR1Set()
{
    string handle;
    for(auto lr0Pair : m_lr0Items)
    {
        for(auto item : lr0Pair.second)
        {
            handle = findNextHandle(item);
            for (auto fol : m_follow[handle])
            {
                m_lr1Items[lr0Pair.first].emplace(LR1RHS(item, fol));
            }
        }
    }
}

// finds every first terminal of state
void StateTableGenerator::closure(string state)
{
    GrammarTree::node* parent = m_grammar.rule(state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {

        if (endOfRule(rule))
            continue;
        //cout << **rule << endl;

        if (*(*rule) == state)
        {
            gotoNextRule(rule);
        }

        else if (isupper((*rule)->at(0)))
        {
            if (!contains(m_terminals, *rule))
                m_terminals.emplace(*rule);
            gotoNextRule(rule);
        }
        else
        {
            if (contains(m_nonterminals, *rule))
            {
                gotoNextRule(rule);
                continue;
            }
            else
            {
                m_nonterminals.emplace(*rule);
            }

            // just for prints
            // cout << **rule << endl;
            if (endOfRule(rule))
                continue;
            // recurse on next node(s)
            closure(**rule);
            while (!endOfRule(rule) && rule != par.end())
                rule++;
        }
    }
}

/****************************************************************/
/*******************INLINE HELPER FUNCTIONS *********************/
/****************************************************************/

// checks if an item exists within a set of strings.
bool StateTableGenerator::contains(set<const string*> list, // set to search.
                                   const string *token) // string to find in the set.
{
    return binary_search(list.begin(), list.end(), token);
}

bool StateTableGenerator::contains(set<string> list, const string token)
{
    return binary_search(list.begin(), list.end(), token);
}

// creates a vector<string> from a vector<string*>
void StateTableGenerator::vecCnstStrPtrToVecStr(const vector<const string*> &vecCnstStrPtr, // vector of string pointers to copy
                                                vector<string> &vecStr) // vector of strings to copy into.
{
    vecStr.clear();
    for (auto strPtr : vecCnstStrPtr)
    {
        vecStr.push_back(*strPtr);
    }
}

// creates a vector<string> from a set<string>
void StateTableGenerator::setStrToVecStr(const set<string> &setStr, // set of strings to copy.
                                         vector<string> &vecStr) // vector of strings to copy into.
{
    vecStr.clear();
    for (auto& str : setStr)
    {
        vecStr.push_back(str);
    }
}

void StateTableGenerator::gotoNextRule(vector<string>::iterator &it)
{
    while ((*it).compare("|") != 0)
        it++;
}
void StateTableGenerator::gotoNextRule(vector<const string*>::iterator &it)
{
    while ((*it)->compare("|") != 0)
        it++;
}
bool StateTableGenerator::endOfRule(vector<string>::iterator &it)
{
    return ((*it).compare("|") == 0);
}
bool StateTableGenerator::endOfRule(vector<const string*>::iterator &it)
{
    return ((*it)->compare("|") == 0);
}
string StateTableGenerator::findNextHandle(string rhs)
{
    size_t start = rhs.find(c_phCh);
    start++;
    size_t end = rhs.find(' ', start);
    return rhs.substr(start, end-start);
}
bool StateTableGenerator::advancePH(string &rhs)
{
    size_t start = rhs.find(c_phCh);
    size_t end;

    if(end = (rhs.find(' ', start) != string::npos))
    {
        rhs.erase(start,1);
        rhs.replace(end, 1, c_phStr);
        return true;
    }
    return false;
}

bool StateTableGenerator::advancePHSets(string &rhs)
{
    size_t start = rhs.find(c_phCh);
    size_t end;

    end = rhs.find(' ', start);
    if (start == (rhs.size() - 1))
    {
        rhs.erase(start,1);
        rhs.replace(end, 1, c_phStr);
        return true;
    }
    return false;
}