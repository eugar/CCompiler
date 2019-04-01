//
// created by Victor Gutierrez on Mar 25, 2019
//
#ifndef CCOMPILER_CPARSER_H
#define CCOMPILER_CPARSER_H

#include "statetables.h"
#include "parsetree.hpp"
#include "token.h"
#include "symboltable.hpp"
#include <vector>

#define LRTABLE "resources/newLrTable.txt"

using namespace std;

typedef pair<StateTables::Action::ACTION, size_t> act;

class Parser
{
public:
    Parser();
    void buildParseTree(ParseTree &parseTree, vector<Token> &tokenList);

private:
    bool runAction(act actRun, ParseTree &parseTree, pnode rule);
    void reduce(ParseTree &parseTree);

    void loadTables(string filepath = LRTABLE);

    void hardcodeTest();

    void createSymbolTable();
    void convertTokenList(vector<Token> tokenList);

    // Gets the action from current state and rule.
    inline act getAction(string rule);
    // Gets the goto state from current state and rule.
    inline size_t getGoto(string rule);
    inline void replaceStack(ParseTree &parseTree, int i);
    inline void printStack();

    vector<size_t> m_stateStack;
    StateTables m_stateTable;
    act m_action;
    pnode m_newRoot;
    vector<pnode> m_nodeStack;
    // <  RHS ,   LHS >
    map<string, string> m_grammarRed;
    vector<pnode> m_tokenStack;
    SymbolTable m_symbolTable;
};

#endif //CCOMPILER_CPARSER_H
