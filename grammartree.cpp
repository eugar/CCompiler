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
    ProductionRule pRule;
    vector<string> pRuleIndex;

    grammar::reader(pRuleIndex, pRule, "expr.txt");

    auto rule = m_tokens.emplace(pRuleIndex[0]);
    m_root.addRule(const_cast<string*>(&(*rule.first)));
    m_root.addParentNode();
    m_root.setParentToken();

    buildTree(pRule);

    cout << endl << "The grammar tree contains " << m_size << " nodes with a root of " << pRuleIndex[0] << endl;

    set<string> LR1Items;
    m_CC.emplace_back(closure(LR1Items));

    constructSets();

}
void GrammarTree::constructSets()
{

    int setCount = 0;
    //while still adding items
    do
    {
        // If there is more than 1 set
        set<string> LR1Items;
        if (setCount > 0)
        {
            vector<string> temp;
            list<string> allCC;
            for (auto CC : m_CC)
            {
                auto iter = allCC.end();
                iter--;
                allCC.splice(iter, list<string>(CC.begin(), CC.end()));
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
        createNewLR1Items(root, root, n->children().back(), phPosition, LR1Items);
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
            // This is a terminal with no rule, Skip.
            if (isupper((*rule)[1]));

            size_t sPos = rule->find(c_phStr);
            size_t ePos;
            if ((ePos = rule->find(' ', sPos)) == string::npos)
                if ((ePos = rule->find('|', sPos)) == string::npos)
                    continue;
            sPos++;

            string s = (rule->substr(sPos, ePos - sPos));
            auto token = m_tokens.find(s);
            if (isupper((*token)[0]))
            {
                // skip
            } else
            {
                clearTokenSets();
                cout << "Token sent to validTerminals : " << *token << endl;
                validTerminals(&(*token));
            }
            if (isupper((*token)[0]))
            {
                continue;
            }

            createNewLR1Items(*token, *rule, m_lhsMap.at(*token), phPosition, LR1Items);
        }

        cout << "LR1Items size : " << LR1Items.size() << endl;
    } while (LR1Items.size() > prevLR1ItemsSize);

    return LR1Items;
}

void GrammarTree::createNewLR1Items(string token, string rule, node* gtNode, int phPosition, set<string> &LR1Item)
{
    // process all terminals first
    for (auto terminal : m_terminals)
    {
        // build LR1
        convertRuleToLR1Item(gtNode->rules(), *terminal, phPosition, LR1Item);
        cout << "Rule : " << token << " -- " << *terminal << endl;
    }
}

void GrammarTree::convertRuleToLR1Item(vector<const string *> vecS, // production rule list
                                                string terminal, // the terminal that should be marked as acceptable
                                                int phPosition, // The position of the placeholder; The number of tokens that come before it.
                                                set<string> &LR1Items)
{
    string production;
    size_t phIndex = 0;
    bool findPos = true;
    bool setInitialPh = true;
    size_t newPhPos = 0;
    for (auto s : vecS)
    {
        if (*s == "EOF" || s->empty())
        {
            production.clear();
            continue;
        }
        auto it = vecS.begin();
        int p = phPosition;
        if (p > 0 && findPos) {
            while (it != vecS.end() && ((*it)->compare("|")))
            {
                newPhPos += (*it)->size() + 1;
                p--;
                if (p == 0)
                {
                    findPos = false;
                    break;
                }
            }

        }
        else if (phPosition == 0 && setInitialPh)
        {
            setInitialPh = false;
            production.insert(0, c_phStr);
        }
        if (*s == "|")
        {
            findPos = true;
            setInitialPh = true;
            newPhPos = 0;
            if (production.empty())
                continue;
            if (production.size() == 1 && production[0] == c_phCh)
                continue;

            // add the acceptable terminal
            production[production.size() - 1] = '|';
            if (newPhPos > 0 && phPosition > 0)
            {
                production.replace(newPhPos, 1, c_phStr);
            }
            production += terminal;

            if ((phIndex = production.find(c_phStr, 0)) == string::npos)
            {
                production.insert(0, c_phStr);
            }
            // place production item in set
            auto it = LR1Items.emplace(production);
            production.clear();
        }
        else
        {
            production += *s + " ";
        }
    }
}
/*
    string production;
    size_t phIndex = 0;
    auto items = set<string>(LR1Items);
    LR1Items.clear();
    bool findPos = true;
    for (auto s : vecS)
    {
        auto it = vecS.begin();
        int p = phPosition;
        size_t newPhPos = 0;
        if (p > 0 && findPos) {
            while (it != vecS.end() && ((*it)->compare("|")))
            {
                newPhPos += (*it)->size() + 1;
                p--;
                if (p == 0)
                {
                    findPos = false;
                    break;
                }
            }

        }
        else if (findPos && it != vecS.end())
        {
            production.insert(0, c_phStr);
        }
        if (*s == "EOF" || s->empty())
        {
            production.clear();
            continue;
        }
        if (*s == "|")
        {
            findPos = true;
            if (production.empty())
                continue;

            // add the acceptable terminal
            production[production.size() - 1] = '|';
            if (!findPos && phPosition > 0)
            {
                production.replace(newPhPos, 1, c_phStr);
            }
            production += terminal;
 */
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
        }

    }
}

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
