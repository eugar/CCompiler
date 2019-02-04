#ifndef PARSER_H
#define PARSER_H

#include "cscanner.hpp"

using namespace std;

// states in the grammar
#define FUNC 50
#define KEY 51
#define BOOL 52
#define TYPE 53
#define STMT 54
#define PRM 55

class Parser
{
public:
    Parser();
    void parseTokens(vector<Token> &tokenList,vector<Expression> &expressionList);
    void printParseTree(vector<Expression> &expressionList);
private:
    vector<int> m_stateList;
    void setState(int i, vector<Token> &tokenList);
    bool isFuncDecl();
};

#endif // PARSER_H
