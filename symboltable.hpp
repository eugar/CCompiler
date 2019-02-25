#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>

using namespace std;


typedef struct
{
    string type;
    string scope;
} symbolData;

class SymbolTable
{

public:

    SymbolTable();
    void insert(string id, string type, string scope);
    int modify(string id, string type, string scope);
    void remove(string id);
    int lookup(string id);
    void printRecords();

private:
    unordered_map<string, symbolData> m_symbolTable;

};

#endif //SYMBOLTABLE_H
