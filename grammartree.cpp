//
// Created by mmalett on 2/16/19.
//

#include "grammartree.h"
#include "grammar.h"
#include <algorithm>
#include <iostream>

GrammarTree::GrammarTree()
    :
    m_size(0)
{
    m_tokens.emplace("|");

    grammar::reader(m_pRuleIndex, m_pRule);

    auto rule = m_tokens.emplace(m_pRuleIndex[0]);
    m_root.addRule(const_cast<string*>(&(*rule.first)));
    m_root.addParentNode();
    m_root.setParentToken();

    buildTree(m_pRule);

    cout << endl << "The grammar tree contains " << m_size << " nodes with a root of " << m_pRuleIndex[0] << endl;
/*
    set<string> LR1Items;
    m_CC.emplace_back(closure(LR1Items));

    constructSets();
    int w = 9;
*/
}
void GrammarTree::constructSets()
{

    //set<string> preppedRules;
    int setCount = 0;
    //while still adding items            sPos++;

    do
    {
        // If there is more than 1 set
        set<string> LR1Items;
        if (setCount > 0)
        {
            vector<string> temp;
            set<string> allCC;
            for (auto& CC : m_CC)
            {
                for (auto C : CC)
                {
                    allCC.emplace(C);
                }
                /*
                for (auto p : preppedRules)
                {
                    allCC.emplace(p);
                }
                 */

            }
            set_difference(m_CCi.begin(), m_CCi.end(), allCC.begin(), allCC.end(), inserter(temp, temp.begin()));
            if (temp.size() <= 0)
            {
                break;
            }
            m_CC.emplace_back(set<string>(std::begin(temp), std::end(temp)));
        }

        LR1Items = set<string>(m_CC.back());

        size_t phPos;
        size_t sPos;
        for (auto &item : LR1Items)
        {
            phPos = item.find(c_phStr);
            if ((sPos = item.find(' ', phPos)) == string::npos)
            {

                if ((sPos = item.find('|', phPos)) == string::npos)
                {
                    continue;
                }
            }
            auto *s = const_cast<string *>(&item);
            s->at(sPos) = c_phCh;
            if (phPos == 0)
            {
                s->erase(phPos, 1);
            }
            else
            {
                s->replace(phPos, 1, " ");
            }

        }

        for (auto &item : LR1Items)
        {
            if ((sPos = item.find('|', phPos)) != string::npos)
            {
                auto *s = const_cast<string *>(&item);
                s->erase(sPos + 1);
            }
        }
        /*
        preppedRules.clear();
        for (auto lr1 : LR1Items)
        {
            size_t pipePos = 0;
            if ((pipePos = lr1.rfind('|')) != string::npos && (pipePos + 1 >= lr1.size()))
            {
                preppedRules.emplace(lr1);
            }
        }
         */
        setCount++;
        m_CCi.clear();
        m_CCi = set<string>(closure(LR1Items, setCount));
    } while(true);
    // wrap it all in the loop.

}
set<string> &GrammarTree::closure(set<string> &LR1Items, int phPosition)
{
    if (LR1Items.empty())
    {
        string root = m_root.children().back()->parentToken();
        node* n = m_lhsMap.at(root);
        vector<const string *> s = n->children().back()->rules();
        //cc.emplace_back(make_pair(convertRuleToLR1Item(n->children().back()->rules(), "", 0), n));
        m_terminals.emplace(&(*m_tokens.find("EOF")));
        convertRuleToLR1Item(root, root, n->children().back(), -1, LR1Items);
        m_terminals.clear();
    }
    size_t prevLR1ItemsSize = 0;
    do
    {
        list<string *> l;
        prevLR1ItemsSize = LR1Items.size();
        for(auto rule = LR1Items.begin(); rule != LR1Items.end(); rule++)
        {
            cout << "Rule : " << *rule << endl;
            //if (!terminals(&(*rule), &token))
              //  continue;

            // This is a terminal with no rule, Skip.

            size_t sPos = rule->find(c_phStr);
            size_t ePos;
            if ((ePos = rule->find(' ', sPos)) == string::npos)
                if ((ePos = rule->find('|', sPos)) == string::npos)
                    continue;
            sPos++;
            cout << endl;
            string s = (rule->substr(sPos, ePos - sPos));
            auto token = m_tokens.find(s);
            if (isupper((*token)[0]))
            {
                size_t n;
                if ((n = rule->find('|')) != string::npos && rule->size() <= (n + 1))
                {
                    size_t n = rule->find_first_of(" \n");
                    cout << "Rule to concat with term : " << *rule + rule->substr(sPos, n - sPos) << endl;
                    LR1Items.emplace(*rule + rule->substr(sPos, n - sPos));
                    LR1Items.erase(rule);

                }
                continue;
            }

            convertRuleToLR1Item(*token, *rule, m_lhsMap.at(*token), phPosition, LR1Items);

            //createNewLR1Items(token, *rule, m_lhsMap.at(token), phPosition, LR1Items);

        }

        cout << "LR1Items size before terminate check : " << LR1Items.size() << endl;
    } while (LR1Items.size() > prevLR1ItemsSize);

    return LR1Items;
}
bool GrammarTree::terminals(const string *rule, string *token)
{
    clearTokenSets();
    // This is a terminal with no rule, Skip.
    if (isupper((*rule)[1]))
    {
        size_t sPos = 0;
        if ((sPos = (*rule).find('|')) != string::npos)
        {
            // the only valid terminal for a terminal is THAT terminal!
            string *s = new string((*rule).substr(sPos + 1, (*rule).find_first_of("| @\n", sPos + 1)));
            m_terminals.emplace(s);
        }
        else
        {
            return false;
        }

    }

    size_t sPos = rule->find(c_phStr);
    size_t ePos;
    cout << "Rule in terminals : " << *rule << endl;
    if ((ePos = rule->find(' ', sPos)) == string::npos)
        if ((ePos = rule->find('|', sPos)) == string::npos)
            return false;
    sPos++;

    string s = (rule->substr(sPos, ePos - sPos));
    *token = *(m_tokens.find(s));
    if (isupper((*token)[0]))
    {
        return false;
    }
    else
    {
        cout << "Token sent to validTerminals : " << *token << endl;
        validTerminals(&(*token));
    }
    *token = s;
    return true;
}
void GrammarTree::createNewLR1Items(string token, string item, node* gtNode, int phPosition, set<string> &LR1Item)
{
    // process all terminals first
    for (auto terminal : m_terminals)
    {
        // build LR1
       // if ((item.find(c_phStr)) != 0)
        //{
        //    convertRuleToLR1Item(gtNode->, *terminal, phPosition, LR1Item, item);
       // } else {
         //   convertRuleToLR1Item(gtNode, *terminal, phPosition, LR1Item, item);
       // }
        cout << "Rule : " << token << " -- " << *terminal << endl;
    }
}
void GrammarTree::convertRuleToLR1Item(string token,
                                        string item,
                                        GrammarTree::node *gtNode,
                                        int phPosition,
                                        set<string> &LR1Items)
{

    vector<string> vecS;
    size_t phIndex = 0;

    bool isNew = m_processingList.emplace(token).second;


    if ((phIndex = item.find(c_phStr)) != 0 && phIndex != string::npos && phPosition > 0)
    {
        size_t ePos = 0;
        size_t sPos = 0;
        while ((ePos = item.find_first_of("@ |", sPos)) != string::npos)
        {
            vecS.push_back(item.substr(sPos, ePos - sPos));
            ePos++;
            sPos = ePos;
        }
        if ((sPos = item.find('|')) != string::npos)
        {
            // place pipe as last string in vector.
            vecS.push_back(item.substr(sPos, 1));
        }
    }
    else
    {
        if (item == "process")
        {
            gtNode = m_lhsMap.at(token);
        }
        for (auto rule : gtNode->rules())
        {
            vecS.push_back(*const_cast<string *>(&(*rule)));
        }
    }
    string production;
    //bool findPos = true;
    //bool setInitialPh = true;
    size_t newPhPos = 0;
    for (auto s : vecS)
    {
        cout << "Current vector string : " << s << endl;
        if (s.empty() || s == "EOF")
        {
            production.clear();
            continue;
        }
        // After this statement, phIndex is either the maximum unsigned value
        // or the index where the ph should be placed.
        phIndex = item.find(c_phStr);

        if (s == "|")
        {
            // findPos = true;
            // setInitialPh = true;
            // newPhPos = 0;
            if (production.empty())
                continue;
            if (production.size() == 1 && production[0] == c_phCh)
                continue;

            production[production.size() - 1] = '|';
            if (phIndex == string::npos || phIndex == 0)
            {
                phIndex = 0;
                production.insert(phIndex, c_phStr);
            }
            else
            {
                production.replace(phIndex, 1, c_phStr);
            }
            if (item == "process")
            {
                item = production;
            }
            if (phPosition != -1)
            {
                terminals(&item, &token);
            }
            for (auto terminal : m_terminals)
            {
                // add the acceptable terminal to the set
                LR1Items.emplace(production + *terminal);
            }
            production.clear();
            for (auto nonterminal : m_nonterminals)
            {
                if ((m_processing = m_processingList.emplace(*nonterminal).second))
                {
                    convertRuleToLR1Item(*nonterminal, "process", m_lhsMap.at(*nonterminal), 0, LR1Items);
                    m_processing = false;
                    // m_nonterminals was cleared due to a failure in terminals.
                    if (m_nonterminals.empty())
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            production += s + " ";
        }

    }
    //if (item[item.size() - 1] == '|')
    //{
     //   LR1Items.erase(item);
//    }
}

GrammarTree::node *GrammarTree::addChild(GrammarTree::node *parentNode,
                                                vector<string> rhs,
                                                const string *parentToken) // lhs
{
    node *child = new node();
    child->addParentNode(parentNode);
    child->setParentToken(const_cast<string*>(parentToken));
    parentNode->addChildNode(child);

    cout  << endl;
    cout << "PrevRule : " <<  m_prevRule  << "-- Rule : " << child->parentToken() <<   "  ~~~ " ;

    for (auto& rule : rhs)
    {
        size_t sPos = 0;
        size_t ePos = 0;
        while ((ePos = rule.find(' ', sPos)) != string::npos)
        {
            // put in token list
            auto r = m_tokens.emplace(rule.substr(sPos, ePos - sPos));
            // keep lhs unique
            string s = *r.first;
                // point to list item from tree
                child->addRule(const_cast<string *>(&(*r.first)));
                cout << " " << (*r.first);
            ePos++;
            sPos = ePos;
        }
        cout << " | ";
        // add rule separator
        child->addRule(&(*m_tokens.find("|")));
    }
    cout << endl;
    m_size++;
    string curRule = child->parentToken();
    if (curRule != m_prevRule)
    {
        m_prevRule = curRule;
        m_lhsMap[m_prevRule] = child;
    }

    return child;
}
/*
// This finds all accepting states if given the root node
void GrammarTree::acceptStates(const string* state)
{
    node* parent = m_lhsMap.at(*state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {
        while ((*rule)->compare("|"))
            rule++;
        rule--;
        //cout << **rule << endl;
        if (*(*rule) == *state)
        {
            // do nothing
        }

        else if (isupper((*rule)->at(0)))
        {
            if (!contains(m_terminals, *rule))
                m_terminals.emplace(*rule);
        }
        else
        {
            if (contains(m_nonterminals, *rule))
                return;
            else
                m_nonterminals.emplace(*rule);

            // recurse on next node(s)
            for (auto nonterm : (m_lhsMap.at(*(*rule)))->rules())
            {
                if (isupper(nonterm->at(0)))
                {
                    m_terminals.emplace(nonterm);
                    continue;
                }
                if (contains(m_nonterminals, nonterm))
                    return;
                //cout << "rule : " << *(*rule) << endl;
                //cout << "nonterm : " << *nonterm << endl;
                if (nonterm->compare(*(*rule)) != 0 &&
                    nonterm->compare("|") != 0)
                {
                    validTerminals(nonterm);
                }
            }
        }
        rule++;
    }
}
*/
// given a string, it returns a list of all valid terminals that can be seen
void GrammarTree::validTerminals(const string* state)
{
    node* parent = m_lhsMap.at(*state);
    auto par = parent->rules();
    for (auto rule = par.begin(); rule != par.end(); rule++)
    {

        if ((*rule)->compare("|") == 0)
            continue;
        cout << **rule << endl;

        if (*(*rule) == *state)
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
                return;
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
            validTerminals(*rule);
            while ((*rule)->compare("|") != 0 && rule != par.end())
                rule++;
        }
    }
    return;
}




            /*
            for (auto nonterm = nonterminal.begin(); nonterm != nonterminal.end(); nonterm++)
            {

                if (isupper((*nonterm)->at(0)))
                {
                    m_terminals.emplace(*nonterm);
                    // move to next rule
                    while ((*nonterm)->compare("|"))
                        nonterm++;
                    continue;
                }
                else if ((*nonterm)->compare(*(*rule)) != 0 &&
                        (*nonterm)->compare("|") != 0)
                {

                    validTerminals(*nonterm);
                    while ((*nonterm)->compare("|"))
                        nonterm++;
                    continue;
                }
                else
                {

                    while ((*nonterm)->compare("|"))
                    {
                        nonterm++;
                    }
                }
            }*/


const vector<const string*> GrammarTree::node::rightChildrensRules()
{
    vector<const string*> right;
    for (auto child : m_children)
    {
        right.emplace_back((child->m_rules.back()));
    }
    return right;
}

bool GrammarTree::contains(set<const string*> list, const string *token)
{
    return binary_search(list.begin(), list.end(), token);
}

GrammarTree::node *GrammarTree::buildTree(ProductionRule &pRule)
{
    node *node = addChild(&m_root, pRule[*m_root.rules().back()], m_root.rules().back());

    // send child nodes to recursively build tree
    return buildTree(pRule, &m_root, (&m_root)->children());
}

GrammarTree::node *GrammarTree::buildTree(ProductionRule &pRule,
                                            GrammarTree::node* parent,
                                            const std::vector<node*> &children)
{
    node* newNode = nullptr;
    for (auto& child : parent->children())
    {
        newNode = buildTree(pRule, child, child, child->rules());
    }

    return &m_root;
}

GrammarTree::node *
GrammarTree::buildTree(ProductionRule &pRule,
                        GrammarTree::node *parent,
                        GrammarTree::node *child,
                        const std::vector<const string *> &rules) // list of rules to build from i.e., the parent's rhs
{
    node* newNode = nullptr;
    for (auto& rule : rules)
    {
        // don't make root nodes from terminals or items already on the LHS
        if (islower((*rule)[0]) && m_lhsMap.find(*rule) == m_lhsMap.end())
        {
            newNode = addChild(child, pRule[*rule], rule);
            newNode = buildTree(pRule, child, newNode->children());
        }

    }
    return newNode;
}
