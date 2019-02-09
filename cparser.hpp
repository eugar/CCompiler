#ifndef PARSER_H
#define PARSER_H

#include "cscanner.hpp"

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
public:
    Parser();
    void parseTokens(vector<Token> tokenList);
    void printParseTree();
private:
    vector<int> m_stateList;
    vector<string> m_printTree;
    void setState(int i, vector<Token> &tokenList);
    bool isFuncDecl();
};

#endif // PARSER_H
