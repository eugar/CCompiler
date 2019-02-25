//
// Created by mmalett on 2/22/19.
//
#include <iostream>
#include <algorithm>
#include "statetablegenerator.h"

using namespace std;

StateTableGenerator::StateTableGenerator()
{
    first();
    follow2();
    buildLRSet();
    findLR0Keys((m_grammar.ruleIndex())[0]);
    buildLR0Set();

    int w;
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

void StateTableGenerator::follow2()
{
    // loop over all left hand side handles.
    for (auto lhs = m_grammar.ruleIndex().begin(); lhs != m_grammar.ruleIndex().end(); lhs++)
    {

        multimap<string, string> rhsFollow;
        set<string> follow;
        findFollowLHS(*lhs, follow);
        findFollowRHS(*lhs, rhsFollow);
        buildFollow(*lhs, follow);
    }
}
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
void StateTableGenerator::findFollowLHS(string lhs, set<string> &follow)
{
    vector<string> rules;
    vecCnstStrPtrToVecStr(m_grammar.rule(lhs)->rules(), rules);
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        if (*rule == lhs)
        {
            rule++;
            if (endOfRule(rule))
            {
                continue;
            }
            else
            {
                follow.emplace(*rule);
                gotoNextRule(rule);
            }
        }
    }
}
// loops over all LHSs and searches the RHSs for instances of each rule.
void StateTableGenerator::findFollowRHS(string nonterminal, multimap<string, string> &rhsFollow)
{
    for (auto& lhs : m_grammar.ruleIndex())
    {
        vector<const string*> rules = m_grammar.rule(lhs)->rules();
        for (auto rule = rules.begin(); rule != rules.end(); rule++)
        {
            //cout << "Rule : " << **rule << endl;
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
                cout << "Trying to add : " << nonterminal << " with "<< **rule << endl;
                rhsFollow.emplace(make_pair(nonterminal, **rule));
            }

        }
    }
    return;
}

void StateTableGenerator::buildLRSet()
{
    for (auto& lhs : m_grammar.ruleIndex())
    {
        auto rules = m_grammar.rule(lhs)->rules();
        string rule;
        for (auto pRule = rules.begin(); pRule != rules.end(); pRule++)
        {
            cout << "pRule : " << **pRule << endl;
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
                m_allLRItems.emplace(std::make_pair(lhs, rule));
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
        auto rules = m_allLRItems.equal_range(lhs);
        for (auto rule = rules.first; rule != rules.second; rule++)
        {
            m_lr0Items.emplace(make_pair(lhs, rule->second));
        }

    }
}

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
/*
void StateTableGenerator::lookAhead(string state, string terminal, vector<string> &follow)
{
    vector<string> rules;
    // make a copy so we don't have to deal with iterators to pointers.
    vecCnstStrPtrToVecStr(m_grammar.rule(state)->rules(), rules);
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        // we are looking for the specific terminal first.
        // then we want to backtrack to all places where there is a follow rule
        if (*rule == state)
        {
            // check to see if a terminal follows the rule
            rule++;
            if (isupper((*rule)[0]))
            {
                // this is a follow symbol. Let findFollow deal with it.
                //m_prevLHS.push_front(terminal);
                gotoNextRule(rule);
            }
            else
            {
                m_prevLHS.push_front(*rule);
                lookAhead(*rule, terminal, follow);
            }
        }
        if (endOfRule(rule))
        {
            continue;
        }
        cout << " State : " << state << " | | | ";
        cout << "Rule : " << *rule << endl;

        if (isupper((*rule)[0]))
        {
            if (*rule == terminal)
            {
                // found the terminal
                m_prevLHS.push_front(terminal);
            }
            else
            {
                continue;
            }
        }
        else
        {
            // add to the stack
            cout << "Added  |> '" << *rule << "' <| to the stack" << endl;
            m_prevLHS.push_front(*rule);
            lookAhead(*rule, terminal, follow);
        }
        gotoNextRule(rule);
    }
}
*/
/*
void StateTableGenerator::backtrack(string lhs, string terminal)
{
    for (auto rule : m_prevLHS)
    {
        if (isupper(rule[0]))
        {
            // this is the first terminal.
            continue;
        }
        else
        {
            findFollow(rule, m_lhs);
        }

    }
    m_prevLHS.clear();
}
*/
/*
void StateTableGenerator::findFollow(string lhs, string handle)
{
   // cout << "lhs : " << lhs << endl;
    auto rules = m_grammar.rule(lhs)->rules();
    for (auto rule = rules.begin(); rule != rules.end(); rule++)
    {
        cout << "findFollow => Rule : " << **rule << endl;
        // if the current rule is the lhs it is self referential
        if (lhs == **rule)
        {
            if (isupper((**rule)[0]))
            {
                m_follow[handle].emplace(**rule);
                gotoNextRule(rule);
                continue;
            }
            else
            {
                closure(**rule);
                for (auto terminal : m_terminals)
                {
                    m_follow[handle].emplace(*terminal);
                }
                clearTokenSets();
            }
        }
        // if the current handle is the lhs and is not self referential.
        else if (handle == lhs)
        {
            gotoNextRule(rule);
            continue;
        }
        rule++;
        if (endOfRule(rule))
        {
            continue;
        }
        if (isupper((**rule)[0]))
        {
            m_follow[handle].emplace(**rule);
        }
        else
        {
            closure(**rule);
            for (auto terminal : m_terminals)
            {
                m_follow[handle].emplace(*terminal);
            }
            clearTokenSets();
        }
        gotoNextRule(rule);

    }
}
*/

/*
void StateTableGenerator::follow()
{
    // get first terminals
    for (auto &lhs : m_grammar.ruleIndex())
    {
        // copy first terminal set into firstTerminals vector
        vector<string> firstTerminals;
        setStrToVecStr(m_first[lhs], firstTerminals);

        lookAhead(lhs, firstTerminals);
        while (!firstTerminals.empty())
        {
            bool isDone = false;
            GrammarTree::node *fol = m_grammar.rule(m_prevLHS.back());
            vector<string> rules;
            // copy into the rules vector
            vecCnstStrPtrToVecStr(fol->rules(), rules);
            cout << "Previous Token : " << m_prevLHS.back() << endl;
            for (auto rule = rules.begin(); rule != rules.end(); rule++)
            {
                cout << "Rule : " << (*rule) << endl;
                rule++;
                if (endOfRule(rule))
                {
                    continue;
                }
                cout << lhs << "[0] : " << *rule << endl;
                if (isupper((*rule)[0]))
                {
                    m_follow[lhs].emplace(*rule);
                    //isDone = true;
                    cout << "Follow placed : " << *rule << " ";
                }

                cout << endl << endl;
                gotoNextRule(rule);
            }
            m_prevLHS.pop_back();
            if (m_prevLHS.empty() || isDone)
            {
                break;
            }
        }
    }
}
*/
/*
void StateTableGenerator::lookAhead(const string state, vector<string> &firstTerminals)
{
    string currentState = "";
    GrammarTree::node* parent = m_grammar.rule(state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {
        if (std::find(m_branchNonTerminals.begin(), m_branchNonTerminals.end(), state) != m_branchNonTerminals.end()
            && !contains(m_branched, state))
        {
            m_branching = true;
            m_branched.emplace(state);
        }


        if (endOfRule(rule))
            continue;
        cout << "The current state is : " << state << " and the rule is : " << **rule << endl;

        if (*(*rule) == state)
        {
            gotoNextRule(rule);
        }
        else if (isupper((*rule)->at(0)))
        {
            if (firstTerminals.empty())
            {
                m_branching = false;
                setStrToVecStr(m_first[state], firstTerminals);

            }
            // add to terminal stack and save the current previous LHS term
            m_terminalStack.push_back(**rule);
            m_nonTerminalStack.push_back(&(m_prevLHS.back()));
            firstTerminals.erase(find(firstTerminals.begin(), firstTerminals.end(), **rule));

            gotoNextRule(rule);

            if (firstTerminals.empty())
            {
                return;
            }
        }
        else
        {
            // just for prints
            cout << **rule << endl;
            if (endOfRule(rule))
                continue;
            // recurse on next node(s)
            // we have a branching non-terminal
            if (!currentState.empty())
            {
                m_branchNonTerminals.push_back(*const_cast<string*>(*rule));
                cout << "Branch Terminal : " << **rule << endl;
                //m_branched.emplace(m_prevLHS.at(m_prevLHS.size() - 2));

            }
            else
            {
                currentState = state;
            }
            // don't add a non-terminal to the stack twice.
            if (find(m_prevLHS.begin(), m_prevLHS.end(), **rule) == m_prevLHS.end())
            {
                m_prevLHS.push_back(state);
            }

            lookAhead(**rule, firstTerminals);

            while (!endOfRule(rule))
            {
                rule++;
            }
            if (rule == par.end())
            {
                currentState.clear();
            }
        }
    }
}
*/
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