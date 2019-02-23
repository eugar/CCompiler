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
    void insertRecord(string id, string type, string scope);
    int modifyRecord(string id, string type, string scope);
    void deleteRecord(string id);
    int lookupRecord(string id);
    symbolData getRecord(string id);

private:
    unordered_map<string, symbolData> m_symbolTable;

};

#endif //SYMBOLTABLE_H
