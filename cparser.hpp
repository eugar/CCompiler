#ifndef PARSER_H
#define PARSER_H
#include "cscanner.hpp"

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
    typedef map<string, vector<string>> ProductionRule;

public:
    Parser();
    void parseTokens(vector<Token> tokenList);
    void printParseTree();

private:
    void setState(int i, vector<Token> &tokenList);
    bool isFuncDecl();

    vector<int> m_stateList;
    vector<string> m_printTree;


    void loadGrammar();
    bool nextRule(string line, int &start, int &length);

    ProductionRule m_pRule;
    vector<string> m_prodRuleIndex;
};

#endif // PARSER_H
