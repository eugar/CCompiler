//
// created by Victor Gutierrez on Mar 25, 2019
//
#include <iostream>
#include "parsetree.hpp"

using namespace std;

ParseTree::ParseTree()
{

}

void ParseTree::printTree()
{
    cout << "[" << m_root.rule() << "]" << endl;
    for(auto child : m_root.children())
    {
        printChildren(child, 1);
    }
}

void ParseTree::reduce(pnode newRoot, vector<pnode>children)
{
    this->newRoot(newRoot);
    for(auto child : children)
    {
        m_root.addChild(child);
    }
}

void ParseTree::printChildren(pnode pn, int count)
{
    for (size_t i = 0; i < count; i++)
    {
        cout << " ";
    }
    cout << "[" << pn.rule() << "]";
    cout << " " << pn.childCount() << endl;
    for(int i = 0; i < pn.childCount(); i++)
    {
        printChildren(pn.children()[i], count + 1);
    }
}
