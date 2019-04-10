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
    pnode(){m_type = ERR;}
    pnode(string rule){m_rule = rule; m_type = ERR;}
    void addParentNode(pnode* parentNode = nullptr)
    {
        m_parentNode = parentNode;
    }
    pnode* parent() {return m_parentNode;}
    void setRule(string rule){m_rule = rule;}
    string rule(){return m_rule;}
    vector<pnode> children(){return m_children;}
    void addChild(pnode child){m_children.push_back(child);}
    size_t childCount(){return m_children.size();}
    void setType(tokType type){m_type = type;}
    tokType type(){return m_type;}
    int lineNo(){return m_lineNo;}
    void setLineNo(int lineNo){m_lineNo = lineNo;}

private:
    struct pnode* m_parentNode;
    vector<pnode> m_children;
    string m_rule;
    tokType m_type;
    int m_lineNo;
}pnode;

class ParseTree
{
public:
    ParseTree();
    void printTree();
    // Reduce multiple "trees" to one tree (root).
    void reduce(pnode newRoot, vector<pnode>children);
    pnode root(){return m_root;}
    // returns the children of root
    vector<pnode> rootChildren(){return m_root.children();}
    void newRoot(pnode newRoot){m_root = newRoot;}

private:
    pnode m_root;
    inline void printChildren(pnode pn, int count);
};

#endif // CCOMPILER_PARSETREE_H
