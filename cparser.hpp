#ifndef PARSER_H
#define PARSER_H
#include "cscanner.hpp"
#include "grammartree.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

// states in the grammar
#define FUNC 50
#define KEY 51
#define BOOL 52
#define DECL 53
#define STMT 54
#define PRM 55
#define TYPE 56
#define EXPN 57
#define RTSTMT 58

class Parser
{
    // grammar building typedefs

public:
    Parser();
    void parseTokens(vector<Tok> tokenList);
    void printParseTree();

private:
    void setState(int i, vector<Tok> &tokenList);
    bool isFuncDecl();

    vector<int> m_stateList;
    vector<string> m_printTree;

    // grammar building functions
    void mapLookAhead();
    void initClosure();
    void closure();
    void constructTables();

    ProductionRule m_pRule;
    vector<string> m_pRuleIndex;

    // this should be sorted for faster access, so build with a set and
    // convert to a vector.
    vector<string> m_terminals;
    map<string, vector<pair<size_t, string>>> m_validTerminals;
    ProductionRulePtr m_CC;

    //GrammarTree *m_gTree;

};

#endif // PARSER_H
