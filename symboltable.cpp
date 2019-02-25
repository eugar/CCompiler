#include "symboltable.hpp"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable()
{

}

//given id info, this will insert the record into the symbol table
void SymbolTable::insertRecord(string id, string type, string scope)
{
    int i;
    if ((i = lookupRecord(id)) == -1)
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

int SymbolTable::modifyRecord(string id, string type, string scope)
{

    return 1;

}

void SymbolTable::deleteRecord(string id)
{
   if (m_symbolTable.erase(id) != 1)
   {
       cout << "Error: element not erased." << endl;
   }
}

int SymbolTable::lookupRecord(string id)
{
    auto it = m_symbolTable.find(id);
    if (it == m_symbolTable.end())
    {
        return -1;
    }
    return 1;
}

symbolData SymbolTable::getRecord(string id)
{
    symbolData data;
    auto pos = m_symbolTable.find(id);
    if (pos != m_symbolTable.end()) // the id was found, return the pos
    {
       return pos->second;
    }
    else
    {
        cout << "id not found." << endl;
        return data;
    }
}
