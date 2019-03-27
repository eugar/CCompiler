//
// created by Victor Gutierrez on Mar 25, 2019
//
#ifndef CCOMPILER_PARSETREE_H
#define CCOMPILER_PARSETREE_H

#include "statetables.h"
#include "token.h"
#include <vector>

using namespace std;

typedef struct pnode
{
public:
    pnode() = default;
    pnode(string rule){m_rule = rule;}
    void addParentNode(pnode *parentNode = nullptr)
    {
        m_parentNode = parentNode;
    }
    void setToken(string rule){m_rule = rule;}
    string rule(){return m_rule;}
    vector<pnode*> children(){return m_children;}
    void addChild(pnode* child){m_children.push_back(child);}
    size_t childCount(){return m_children.size();}

private:
    struct pnode* m_parentNode;
    vector<pnode*> m_children;
    string m_rule;
}pnode;

class ParseTree
{
public:
    ParseTree();
    void printTree();
    // Reduce multiple "trees" to one tree (root).
    void reduce(pnode* newRoot, vector<pnode*>children);
    pnode* root(){return m_root;}
    // returns the children of root
    vector<pnode*> rootChildren(){return m_root->children();}

private:
    pnode* m_root;
    void newRoot(pnode* newRoot){m_root = newRoot;}

    inline void printChildren(pnode* pn, int count);
};

#endif // CCOMPILER_PARSETREE_H
