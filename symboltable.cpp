#include "symboltable.hpp"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable()
{
    // root is global scope
    m_parent = NULL;
}

//given id info, this will insert the record into the symbol table
void SymbolTable::insert(string id, string type, void* data)
{
    // will insert symbol if it doesn't exist in current scope
    if (!isTaken(id))
    {
        symbol *sym;
        sym->type = type;
        sym->data = data;
        m_symbolTable.insert(pair<string, symbol*>(id, sym));
    }
    else
    {
        cout << "id already taken." << endl;
    }
}

// modifies the data in a variable
int SymbolTable::modify(string id, void* data)
{
    symbol *sym = lookup(id);
    if (sym == NULL) {
        return 0;         // return 0 if id does not exist
    }
    else
    {
        sym->data = data;
        return 1;        // return 1 if id is successfully mod
    }
}

void SymbolTable::remove(string id)
{
   if (m_symbolTable.erase(id) != 1)
   {
       cout << "Error: element not erased." << endl;
   }
}

// Looks for id in current scope
// Goes to parent scope if not found
symbol* SymbolTable::lookup(string id)
{
    auto sym = m_symbolTable.find(id);
    if (sym != m_symbolTable.end())
    {
        return sym->second;
    }
    else if(sym == m_symbolTable.end() && m_parent != NULL)
    {
        return m_parent->lookup(id);
    }
    else
    {
        // returns null if id is not found
        return NULL;
    }
}

void SymbolTable::printRecords()
{
    for (auto sym = m_symbolTable.begin(); sym != m_symbolTable.end(); sym++)
    {
        cout << sym->first << " " << sym->second->type << endl;
    }
}

// Creates a new scope as a child of the current scope
SymbolTable* SymbolTable::addChild(string tableId, SymbolTable *child)
{
    this->m_childTable.insert(pair<string, SymbolTable*>(tableId, child));

    child->addParent(this);
    return child;
}

// Deletes a child scope and return a pointer to the parent
SymbolTable* SymbolTable::deleteChild()
{
    if (this->m_parent == NULL)
    {
        cout << "Deleting lobal scope" << endl;
        return NULL;
    }

    for(auto &symbol: m_symbolTable)
    {
        remove(symbol.first);
    }

    for(auto &child: m_childTable)
    {
        child.second = NULL;
        m_childTable.erase(child.first);
    }

    return this->m_parent;
}

/**********************Private Functions**********************/
void SymbolTable::addParent(SymbolTable *parent)
{
    m_parent = parent;
}

// Returns true if taken, false if not
bool SymbolTable::isTaken(string id)
{
    auto sym = m_symbolTable.find(id);

    if (sym == m_symbolTable.end()) {
        return false;
    }

    return true;
}
