// a tree to hold the grammar

#ifndef CCOMPILER_GRAMMARTREE_H
#define CCOMPILER_GRAMMARTREE_H

#include <vector>
#include <string>
#include <set>
#include "definitions.h"

using namespace std;

class GrammarTree
{
public:
    typedef struct node
    {
        node() = default;

        void addRule(const string *rule){m_rules.push_back(rule);}
        void addChildNode( node *childNode = nullptr){m_children.emplace_back(childNode);}
        void addParentNode(node *parentNode = nullptr){m_parentNode = parentNode;}
        void setParentToken(string *parentToken = nullptr){m_parentToken = parentToken;}

        // allow access to set rules
        vector<const string*> rules(){return m_rules;}

        // returns a copy of the parent token's string
        const string parentToken(){return *m_parentToken;}

        const vector<node*>& children(){return m_children;}
        const vector<const string*> rightChildrensRules();
        const node* parent(){return m_parentNode;}

    private:
        vector<vector<string*>> m_tokenChildren; // not implemented
        vector<const string*> m_rules;
        vector<node*> m_children;
        string *m_parentToken;
        struct node *m_parentNode;

    }node;

public:

    GrammarTree();

    node* buildTree(ProductionRule& pRule);
    bool contains(const string token);
    set<string>& tokens(){return m_tokens;}
    node* root(){return &m_root;}
    size_t size(){return m_size;}
    node* addChild(node* parentNode, vector<string> rhs, const string *parentToken);



private:
    node* buildTree(ProductionRule& pRule, node* parent, const std::vector<node*> &children);
    node* buildTree(ProductionRule& pRule, node* parent, GrammarTree::node * child, const std::vector<const string*> &children);
    node m_root;
    set<string> m_tokens;
    map<string, node*> m_lhsMap;
    size_t m_size;

    string m_prevRule;
};

#endif //CCOMPILER_GRAMMARTREE_H
