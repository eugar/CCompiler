#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <map>

using namespace std;


typedef struct
{
    string type;
    string data;
} symbol;

class SymbolTable
{

public:

    SymbolTable();
    void insert(string id, string type, string data);
    int modify(string id, string data);
    void remove(string id);
    symbol* lookup(string id);
    void printRecords();
    SymbolTable* addChild(string tableId, SymbolTable *child);
    // delete a scope and go back to the parent scope
    SymbolTable* deleteChild();

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
    void inline addParent(SymbolTable *parent);
    bool inline isTaken(string id);
};

#endif //SYMBOLTABLE_H
