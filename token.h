//
// Created by Eugene on 2019-02-26.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

using namespace std;

typedef enum {
    ID, INT, DUB, SHORT, FLOAT, CHAR, DQUOT, SQUOT, PLUS, MINUS, MUL, DIV, PP, MM, PLUSASGN, MINASGN, MULASGN, DIVASGN, ASGN, OR, GOTO, IF, ELSE, NOT, RET, BRK, WHILE, OPAREN, CPAREN, OBRACE, CBRACE, OBRACK, CBRACK, SEMI, NUMCONST, LSTHN, GRTHN, LSTHEQ, GRTHEQ, EQUAL, AND, NOTEQ, SCOMM, BCOMM, COMMA, EMPTY_PARAM, ERR
} tokType;

class Token
{
public:

    Token(string literal = "", tokType token = ERR, int lineNo = -1);
    static void printTokens(vector<Token> tokens);

    tokType getType()
    {
        return this->token;
    }

    string getLiteral()
    {
        return this->literal;
    }

    int getLineNo()
    {
        return this->lineNo;
    }

private:
    string literal;
    tokType token;
    int lineNo;
    void printToken();
};


#endif //TOKEN_H
