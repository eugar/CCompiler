//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "parsetree.hpp"

using namespace std;

ParseTree::ParseTree()
{
    m_root = nullptr;
}

void ParseTree::printTree()
{
    cout << m_root->token() << endl;
    for(auto child : m_root->children())
    {
        printChildren(child, 1);
    }
}

void ParseTree::reduce(pnode* newRoot, vector<pnode*>children)
{
    this->newRoot(newRoot);
    for(auto child : children)
    {
        m_root->addChild(child);
    }
}

void ParseTree::printChildren(pnode* pn, int count)
{
    for (size_t i = 0; i < count; i++) {
        cout << "\t";
    }
    cout << pn->token() << endl;

    if (pn->childCount() > 0) {
        for(auto child : pn->children())
        {
            printChildren(child, count + 1);
        }
    }
}
