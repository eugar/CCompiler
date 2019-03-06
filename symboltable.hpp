#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <map>

using namespace std;


typedef struct
{
    string type;
    void* data;
} symbol;

class SymbolTable
{

public:

    SymbolTable();
    void insert(string id, string type, void *data);
    int modify(string id, void* data);
    void remove(string id);
    symbol* lookup(string id);
    void printRecords();
    SymbolTable* addChild(string tableId, SymbolTable *child);

private:
    // Private members
    // Holds all the variables in this table
    unordered_map<string, symbol*> m_symbolTable;
    // childs are new scope within the parent
    // parent cannot access child scope
    map<string, SymbolTable*> m_childTable;
    // root is global scope
    SymbolTable *m_parent;

    // Private Functions
    void addParent(SymbolTable *parent);
    bool isTaken(string id);
};

#endif //SYMBOLTABLE_H
