#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

using namespace std;

#define ID 1
#define INT 2
#define DUB 3
#define SHORT 4
#define FLOAT 5
#define CHAR 6
#define DQUOT 7
#define SQUOT 8
#define PLUS 9
#define MINUS 10
#define MUL 11
#define DIV 12
#define PP 13
#define MM 14
#define PLUSASGN 15
#define MINASGN 16
#define MULASGN 17
#define DIVASGN 18
#define ASGN 19
#define OR 20
#define GOTO 21
#define IF 22
#define ELSE 23
#define NOT 24
#define RET 25
#define BRK 26
#define WHILE 27
#define OPAREN 28
#define CPAREN 29
#define OBRACE 30
#define CBRACE 31
#define OBRACK 32
#define CBRACK 33
#define SEMI 34
#define NUMCONST 35
#define LSTHN 36
#define GRTHN 37
#define LSTHEQ 38
#define GRTHEQ 39
#define EQUAL 40
#define AND 41



typedef pair<int,string> Tok;

class Expression
{

public:
    Expression() : m_label(""){};

    // second constructor that we might need in the
    // future when we add scope.
    Expression(int scope) : m_label(""), m_scope(scope){};

    // constructor for imbeded expressions
    Expression(int scope, Expression *exp);

    void setLabel(string label){m_label = label;}
    string getLabel(){return m_label;}
    int getScope(){return m_scope;}

private:
    Expression *m_expression;
    string m_label;
    int m_scope;

};

class Variable
{

public:
    Variable(string name, int scope);
    Variable(string name, int scope, void* data);

    string getName(){return m_name;}
    int getScope(){return m_scope;}

    void* getData(){return m_data;}
    void setData(void* data) {m_data = data;}


private:
    string m_name;
    int m_scope;
    void *m_data;

};

#endif // EXPRESSION_H
