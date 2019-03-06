//
// Created by mmalett on 2/22/19.
//
#include <iostream>
#include <algorithm>
#include "statetablegenerator.h"

using namespace std;

size_t Lr1Item::states = 0;

// StateTableGenerator constructor
StateTableGenerator::StateTableGenerator()
{
    first();
    follow();
    buildLr1Sets();
    createStates();
    createTables();
    writeTables();

    int w;
}
void StateTableGenerator::createTables()
{
    StateTables tables;
    tables.generateTables(m_ccCurSets, m_ccPrevSets, m_action, m_goto);
}
void StateTableGenerator::writeTables()
{

}
void StateTableGenerator::buildLr1Sets()
{
    // Place goal rule
    m_lr1ItemSet.emplace(m_grammar.ruleIndex()[0], m_grammar.ruleIndex()[1], "EOF");
    for (auto& item : m_lr1ItemSet)
    {
        m_lr1ItemIndex[m_grammar.ruleIndex()[0]].emplace(const_cast<Lr1Item*>(&item));
    }

    auto& rules = m_grammar.ruleIndex();
    for (auto lhs = (rules.begin() + 1); lhs != rules.end(); lhs++)
    {
        bool noFollow = true;
        vector<string> rhsRules = rhsToLr1ItemFormat(*lhs);
        for (auto &rhs : rhsRules)
        {m_lr1ItemSet.emplace(m_grammar.ruleIndex()[0], m_grammar.ruleIndex()[1], "EOF");
    for (auto& item : m_lr1ItemSet)
    {
        m_lr1ItemIndex[m_grammar.ruleIndex()[0]].emplace(const_cast<Lr1Item*>(&item));
    }
            for (auto &follow : m_follow[findNextHandle(rhs)])
            {
                noFollow = false;
                m_lr1ItemSet.emplace(*lhs, rhs, follow);
            }

            if (noFollow)
            {
                m_lr1ItemSet.emplace(*lhs, rhs);
            }
        }
        for (auto& lr1Item : m_lr1ItemSet)
        {
            Lr1Item *lr1 = const_cast<Lr1Item*>(&lr1Item)->getByLhs(*lhs);

            if (lr1 != nullptr)
            {
                m_lr1ItemIndex[*lhs].emplace(lr1);
            }
        }
    }

    return;
}

void StateTableGenerator::initialLr1Set()
{
    // Build root item
    Lr1Item rootItem(m_grammar.ruleIndex()[0], (rhsToLr1ItemFormat(m_grammar.ruleIndex()[0]))[0], "EOF");
    // Place into Canonical Collection
    m_lr1CC.emplace(rootItem);
    // Add to state matching sets for later (action/goto table construction).
    m_ccCurSets[0].emplace(rootItem);
    // This is not strictly necessary to add here, but it lets us know if
    // something went wrong such as if some sets weren't added properly.
    m_ccPrevSets[0].emplace(rootItem);

}

void StateTableGenerator::generateLr1SetClosures()
{
    size_t prevSize = 0;
    do
    {
        prevSize = m_lr1CC.size();
        for (auto lr1Item = m_lr1CC.begin(); lr1Item != m_lr1CC.end(); lr1Item++)
        {
            string handle = findNextHandle(lr1Item->rhs, lr1Item->phPos);

            // If the token following the placeholder is the same as the lhs
            if (handle == lr1Item->lhs)
            {
                // add new follow set:
                string token = lr1Item->handleFollow();
                // This would only occur if the grammar is incorrect, but checking anyway.
                if (token.empty())
                {
                    continue;
                }
                // If a terminal follows the current handle set that terminal as a follow for all rules of the current handle.
                if (isupper(token[0]))
                {
                    for (auto item : m_lr1ItemIndex[lr1Item->lhs])
                    {
                        m_lr1CC.emplace(Lr1Item(item, token));
                    }
                }
                // If a non-terminal follows the current handle, get the FIRST set for that nonterminal and place each
                // terminal as a follow on the current handle's rules.
                else
                {
                    for (auto item : m_lr1ItemIndex[lr1Item->lhs])
                    {
                        for (auto& terminal : m_first[token])
                        {
                            m_lr1CC.emplace(Lr1Item(item, terminal));
                        }
                    }
                }
            }
            // If the token is a terminal, or if the ph is at the end of the rhs string then we are done with this item.
            // Otherwise, we need to find the closure and create new items until we reach a terminal.
            else if (!isupper(handle[0]) && !lr1Item->phAtEnd())
            {
                closure(handle);
                for (auto rhs : rhsToLr1ItemFormat(handle))
                {
                    m_lr1CC.emplace(handle, rhs, lr1Item->lookAhead, lr1Item->state);
                }
                for (auto& nt : m_nonterminals)
                {
                    for (auto& rhs : rhsToLr1ItemFormat(*nt))
                    {
                        m_lr1CC.emplace(*nt, rhs, lr1Item->lookAhead, lr1Item->state);
                    }
                }
            }
        }
    }
    while(m_lr1CC.size() > prevSize);
}

// Creates the canonical set of LR1 Items.
void StateTableGenerator::createStates()
{
    set<Lr1Item, Lr1Compare> CCi;

    // Build the root item.
    initialLr1Set();

    size_t prevSize;
    do
    {
        // Record the current number of LR1 Items.
        prevSize = m_lr1CC.size();

        // Run the current set through closure
        generateLr1SetClosures();
        // The new items from closure (if any) were added to the main
        // canonical collection variable, m_lr1CC. Remove them from this
        // set to prepare the next.
        CCi.clear();
        // Lr1Item::advancePh requires an empty map to track states while
        // advancing the ph. Its lifetime is the same as this scope.
        map<pair<string, size_t>, size_t> stateMap;

        // Iterate on all current items in the canonical collection and advance
        // the ph for any item that has not reached the end.
        for (auto& item : m_lr1CC)
        {
            Lr1Item newLr1Item;
            if (!item.phAtEnd() &&
            !(newLr1Item = (const_cast<Lr1Item*>(&item))->advancePh(stateMap)).empty() )
            {
                auto result = CCi.emplace(newLr1Item);
            }
        }
        for (auto& item : CCi)
        {
            auto result = m_lr1CC.insert(item);
            // Add the item to state maps for later use in constructing
            // the tables.
            m_ccCurSets[item.nextState].emplace(item);
            m_ccPrevSets[item.fromState].emplace(item);
        }
    // Has the number of LR1 Items increased? If yes, go around again.
    } while(m_lr1CC.size() > prevSize);
    // state test print
    set<size_t> states;
    for (auto item : m_lr1CC)
    {
        states.emplace(item.state);
    }
    cout << "There are " << states.size() << " unique states. They are numbered as follows:" << endl << "States : ";
    for (auto state : states)
    {
        cout << state << " ";
    }
    cout << endl;
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
void StateTableGenerator::follow()
{
    set<string> nonterms;
    // loop over all left hand side handles.
    for (auto lhs = m_grammar.ruleIndex().begin(); lhs != m_grammar.ruleIndex().end(); lhs++)
    {
        set<string> follow;
        findFollowRHS(*lhs, follow);
        nonterms.clear();
       // findFollowRecurse(*lhs, follow, nonterms);
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

// Finds every first terminal of a symbol and any non terminals that
// lead to it.
void StateTableGenerator::closure(string symbol)
{
    GrammarTree::node* parent = m_grammar.rule(symbol);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {

        if (endOfRule(rule))
            continue;
        //cout << **rule << endl;

        if (*(*rule) == symbol)
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

string StateTableGenerator::findNextHandle(string rhs, size_t phPos)
{
    size_t start = phPos;
    if (phPos > 0)
    {
        start++;
    }
    size_t end = rhs.find(' ', start );
    return rhs.substr(start, end-start);
}

vector<string> StateTableGenerator::rhsToLr1ItemFormat(string lhsRule)
{
    vector<string> rules;
    vecCnstStrPtrToVecStr(m_grammar.rule(lhsRule)->rules(), rules);
    string rhs;
    vector<string> rhsRules;
    for (auto& rule : rules)
    {
        if (rule == "|")
        {
            if (!rhs.empty())
            {
                // delete last space
                rhs.erase(rhs.size() - 1, 1);
                rhsRules.push_back(rhs);
            }
            rhs.clear();
            continue;
        }
        else
        {
            rhs += rule + " ";
        }
    }
    return rhsRules;
}

