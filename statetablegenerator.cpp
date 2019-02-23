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
    follow();
    int w;
}
// Given a string, it returns a list of all valid terminals that can be seen
// Loops over 'first()' for every LHS rule.
void StateTableGenerator::first()
{
    for (auto &lhs : m_grammar.ruleIndex())
    {
        closure(lhs);
        set<string> rhs;
        for (auto terminal : m_terminals)
        {
            rhs.emplace(*terminal);
        }
        m_first[lhs] = rhs;
        clearTokenSets();
    }
}

void StateTableGenerator::follow()
{
    for (auto &lhs : m_grammar.ruleIndex())
    {
        lookAhead(lhs);
        while (true)
        {
            bool isDone = false;
            GrammarTree::node *fol = m_grammar.rule(m_prevLHS.back());
            vector<string> rules;
            for (auto r : fol->rules())
            {
                rules.push_back(*r);
            }
            cout << "Previous Token : " << m_prevLHS.back() << endl;
            for (auto rule = rules.begin(); rule != rules.end(); rule++)
            {
                cout << (*rule) << endl;
                rule++;
                if (*rule == "|")
                {
                    continue;
                }
                cout << *rule << "[0] : ";
                for (auto follow : m_first[*rule])
                {
                    m_follow[lhs].emplace(follow);
                    isDone = true;
                    cout << follow << " ";
                }

                cout << endl << endl;
                while (*rule != "|")
                {
                    rule++;
                }
            }
            m_prevLHS.pop_back();
            if (m_prevLHS.empty() || isDone)
            {
                break;
            }
        }
    }
}

void StateTableGenerator::closure(string state)
{
    GrammarTree::node* parent = m_grammar.rule(state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {

        if ((*rule)->compare("|") == 0)
            continue;
        cout << **rule << endl;

        if (*(*rule) == state)
        {
            while ((*rule)->compare("|"))
                rule++;
            //rule--;
        }

        else if (isupper((*rule)->at(0)))
        {
            if (!contains(m_terminals, *rule))
                m_terminals.emplace(*rule);
            while ((*rule)->compare("|"))
                rule++;
        }
        else
        {
            if (contains(m_nonterminals, *rule))
            {
                while ((*rule)->compare("|"))
                    rule++;
                continue;
            }
            else
            {
                m_nonterminals.emplace(*rule);
            }

            // just for prints
            cout << **rule << endl;
            if ((*rule)->compare("|") == 0)
                continue;
            // recurse on next node(s)
            closure(**rule);
            while ((*rule)->compare("|") != 0 && rule != par.end())
                rule++;
        }
    }
}
void StateTableGenerator::lookAhead(const string state)
{
    GrammarTree::node* parent = m_grammar.rule(state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {

        if ((*rule)->compare("|") == 0)
            continue;
        cout << **rule << endl;

        if (*(*rule) == state)
        {
            while ((*rule)->compare("|"))
                rule++;
            //rule--;
        }

        else if (isupper((*rule)->at(0)))
        {
            return;
        }
        else
        {
            // just for prints
            cout << **rule << endl;
            if ((*rule)->compare("|") == 0)
                continue;
            // recurse on next node(s)
            m_prevLHS.push_back(state);
            lookAhead(**rule);
            while ((*rule)->compare("|") != 0 && rule != par.end())
                rule++;
        }
    }
}

bool StateTableGenerator::contains(set<const string*> list,
                                    const string *token)
{
    return binary_search(list.begin(), list.end(), token);
}