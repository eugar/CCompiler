#include "symboltable.hpp"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable()
{

}

//given id info, this will insert the record into the symbol table
void SymbolTable::insert(string id, string type, string scope)
{
    int i;
    if ((i = lookup(id)) == -1)
    {
        symbolData data;
        data.type = type;
        data.scope = scope;
        m_symbolTable.insert(pair<string, symbolData>(id, data));
    }
    else
    {
        cout << "id already taken." << endl;
    }
}

int SymbolTable::modify(string id, string type, string scope)
{

    return 1;

}

void SymbolTable::remove(string id)
{
   if (m_symbolTable.erase(id) != 1)
   {
       cout << "Error: element not erased." << endl;
   }
}

int SymbolTable::lookup(string id)
{
    auto it = m_symbolTable.find(id);
    if (it == m_symbolTable.end())
    {
        return -1;
    }
    return 1;
}

void SymbolTable::printRecords()
{
    for (auto i = m_symbolTable.begin(); i != m_symbolTable.end(); i++)
    {
        cout << i->first << " " << i->second.type << " " << i->second.scope << endl;
    }
}
