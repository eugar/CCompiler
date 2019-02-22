// a tree to hold the grammar

#ifndef CCOMPILER_GRAMMARTREE_H
#define CCOMPILER_GRAMMARTREE_H

#include <vector>
#include <list>
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
    bool contains(set<const string*> list, const string *token);
    set<string>& tokens(){return m_tokens;}
    node* root(){return &m_root;}
    size_t size(){return m_size;}
    node* addChild(node* parentNode, vector<string> rhs, const string *parentToken);
    void validTerminals(const string* terminal);

    void clearTokenSets(){m_nonterminals.clear(); m_terminals.clear();}

    set<string> &closure(set<string> &LR1Item, int phPosition = 0);
    void constructSets();

private:
    void convertRuleToLR1Item(string token, string item, node* gtNode, int phPosition, set<string> &LR1Item);
    //void convertRuleToLR1Item(/*vector<const string *> vecS*/ node *gtNode, string terminal, int phPosition,
    // set<string> &LR1Item, string item);
    void createNewLR1Items(string token, string rule, node* gtNode, int phPosition, set<string> &LR1Item);

    // Loops in grammar can cause infinite recursion.
    // These two sets help to prevent recursing on the
    // same terminal or non-terminal in an endless loop.
    // Must be cleared each time validTerminals is called
    // or validTerminals' behavior is undefined.
    // Also used in the closure algorithm.
    set<const string*> m_terminals;
    set<const string*> m_nonterminals;
    bool terminals(const string *rule, string *token);

    // Holds the canonical collection of LR1 Items
    vector<set<string>> m_CC;
    set<string> m_CCi;

    node* buildTree(ProductionRule& pRule, node* parent, const std::vector<node*> &children);
    node* buildTree(ProductionRule& pRule, node* parent, GrammarTree::node * child, const std::vector<const string*> &children);
    // Root node of the GrammarTree
    node m_root;

    // Unique set of tokens
    set<string> m_tokens;

    // Map of lhs to its production rules (child node)
    map<string, node*> m_lhsMap;

    // Total nodes in the GrammarTree
    size_t m_size;

    string m_prevRule;
};

#endif //CCOMPILER_GRAMMARTREE_H
