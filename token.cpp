//
// Created by Eugene on 2019-02-26.
//

#include <vector>
#include <map>
#include <iostream>
#include "token.h"

using namespace std;

Token::Token(string literal, tokType token, int lineNo)
{
    this->literal = literal;
    this->token = token;
    this->lineNo = lineNo;
}

// print the token information associated with the token that called this function
void Token::printToken()
{
    string token = this->literal;
    tokType type = this->token;
    int lineNo = this->lineNo;
    switch (type) {
        case INT:
            //expression.setLabel("");
            cout << "<" << token << ", integer declaration, (" << lineNo << ")>\n" << endl;
            break;
        case DUB:
            cout << "<" << token << ", double declaration, (" << lineNo << ")>\n" << endl;
            break;
        case SHORT:
            cout << "<" << token << ", short declaration, (" << lineNo << ")>\n" << endl;
            break;
        case FLOAT:
            cout << "<" << token << ", float declaration, (" << lineNo << ")>\n" << endl;
            break;
        case CHAR:
            cout << "<" << token << ", character declaration, (" << lineNo << ")>\n" << endl;
            break;
        case PLUS:
            cout << "<" << token << ", plus symbol, (" << lineNo << ")>\n" << endl;
            break;
        case MINUS:
            cout << "<" << token << ", minus symbol, (" << lineNo << ")>\n" << endl;
            break;
        case MUL:
            cout << "<" << token << ", multiplication, (" << lineNo << ")>\n" << endl;
            break;
        case DIV:
            cout << "<" << token << ", division, (" << lineNo << ")>\n" << endl;
            break;
        case PP:
            cout << "<" << token << ", plus plus, (" << lineNo << ")>\n" << endl;
            break;
        case MM:
            cout << "<" << token << ", minus minus, (" << lineNo << ")>\n" << endl;
            break;
        case PLUSASGN:
            cout << "<" << token << ", plus assignment, (" << lineNo << ")>\n" << endl;
            break;
        case MINASGN:
            cout << "<" << token << ", minus assignment, (" << lineNo << ")>\n" << endl;
            break;
        case MULASGN:
            cout << "<" << token << ", multiply assignemnt, (" << lineNo << ")>\n" << endl;
            break;
        case DIVASGN:
            cout << "<" << token << ", division assignment, (" << lineNo << ")>\n" << endl;
            break;
        case ASGN:
            cout << "<" << token << ", assignment, (" << lineNo << ")>\n" << endl;
            break;
        case GOTO:
            cout << "<" << token << ", terrible programming, (" << lineNo << ")>\n" << endl;
            break;
        case IF:
            cout << "<" << token << ", if statement, (" << lineNo << ")>\n" << endl;
            break;
        case ELSE:
            cout << "<" << token << ", else statement, (" << lineNo << ")>\n" << endl;
            break;
        case NOT:
            cout << "<" << token << ", negation, (" << lineNo << ")>\n" << endl;
            break;
        case RET:
            cout << "<" << token << ", return statement, (" << lineNo << ")>\n" << endl;
            break;
        case BRK:
            cout << "<" << token << ", break statement, (" << lineNo << ")>\n" << endl;
            break;
        case WHILE:
            cout << "<" << token << ", while loop, (" << lineNo << ")>\n" << endl;
            break;
        case OPAREN:
            cout << "<" << token << ", opening parenthesis, (" << lineNo << ")>\n" << endl;
            break;
        case CPAREN:
            cout << "<" << token << ", closing parenthesis, (" << lineNo << ")>\n" << endl;
            break;
        case OBRACE:
            cout << "<" << token << ", opening brace, (" << lineNo << ")>\n" << endl;
            break;
        case CBRACE:
            cout << "<" << token << ", closing brace, (" << lineNo << ")>\n" << endl;
            break;
        case OBRACK:
            cout << "<" << token << ", opening bracket, (" << lineNo << ")>\n" << endl;
            break;
        case CBRACK:
            cout << "<" << token << ", closing bracket, (" << lineNo << ")>\n" << endl;
            break;
        case SEMI:
            cout << "<" << token << ", semicolon, (" << lineNo << ")>\n" << endl;
            break;
        case NUMCONST:
            cout << "<" << token << ", number constant, (" << lineNo << ")>\n" << endl;
            break;
        case ID:
            cout << "<" << token << ", identifier, (" << lineNo << ")>\n" << endl;
            break;
        case LSTHN:
            cout << "<" << token << ", less than, (" << lineNo << ")>\n" << endl;
            break;
        case GRTHN:
            cout << "<" << token << ", greater than, (" << lineNo << ")>\n" << endl;
            break;
        case GRTHEQ:
            cout << "<" << token << ", greater than or equal to, (" << lineNo << ")>\n" << endl;
            break;
        case LSTHEQ:
            cout << "<" << token << ", less than or equal to, (" << lineNo << ")>\n" << endl;
            break;
        case EQUAL:
            cout << "<" << token << ", equals, (" << lineNo << ")>\n" << endl;
            break;
        case AND:
            cout << "<" << token << ", and, (" << lineNo << ")>\n" << endl;
            break;
        case OR:
            cout << "<" << token << ", or, (" << lineNo << ")>\n" << endl;
            break;
        case DQUOT:
            cout << "<" << token << ", double quote, (" << lineNo << ")>\n" << endl;
            break;
        case SQUOT:
            cout << "<" << token << ", single quote, (" << lineNo << ")>\n" << endl;
            break;
        case SCOMM:
            cout << "<" << token << ", line comment, (" << lineNo << ")>\n" << endl;
            break;
        case BCOMM:
            cout << "<" << token << ", block comment, (" << lineNo << ")>\n" << endl;
            break;
        case NOTEQ:
            cout << "<" << token << ", not equal, (" << lineNo << ")>\n" << endl;
            break;
        case COMMA:
            cout << "<" << token << ", comma, (" << lineNo << ")>\n" << endl;
            break;
        default:
            cout << "<" << token << ", Not in the set of known instructions>\n" << endl;
            break;
    }
}

// iterate through the array of Token objects and print information for each individual token
void Token::printTokens(vector<Token> tokens)
{
    for (Token tok : tokens)
    {
        tok.printToken();
    }
}
