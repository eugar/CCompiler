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
    ProductionRule pRule;
    vector<string> pRuleIndex;

    grammar::reader(pRuleIndex, pRule);

    auto rule = m_tokens.emplace(pRuleIndex[0]);
    m_root.addRule(const_cast<string*>(&(*rule.first)));
    m_root.addParentNode();
    m_root.setParentToken();

    buildTree(pRule);

    cout << endl << "The grammar tree contains " << m_size << " nodes with a root of " << pRuleIndex[0] << endl;
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

const vector<const string*> GrammarTree::node::rightChildrensRules()
{
    vector<const string*> right;
    for (auto child : m_children)
    {
        right.emplace_back((child->m_rules.back()));
    }
    return right;
}

bool GrammarTree::contains(const string token)
{
    return binary_search(m_tokens.begin(), m_tokens.end(), token);
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
    //cout << "Parent address : " << parent<< endl;
    //cout << "Child Node : " << parent->children().back() << endl;
    for (auto& child : parent->children())
    {
        newNode = buildTree(pRule, child, child, child->rules());
    }

    // debug
    /*
    if (newNode)
        for (auto& ch : newNode->children())
        {
            for(auto& rule : ch->rules())
            {
                cout << "Child rule : " << *rule << endl;
                cout << "Parent token : " << ch->parentToken() << endl;
            }
        }
        */
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
