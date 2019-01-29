#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

using namespace std;

#define ID 1 //not in cinstructions
#define INT 2
#define DUB 3
#define SHORT 4
#define FLOAT 5
#define CHAR 6
#define FUNC 7 //not in cinstructions
#define KEY 8 //not in cinstructions
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
#define BOOL 20 //not in cinstructions
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
#define OR 42
#define DQUOT 43
#define SQUOT 44

typedef pair<int,string> Token;

class Expression 
{
private:
    Expression *exp;
    string label;
    int scope;

public:
    Expression() : label(""){};

    // second constructor that we might need in the
    // future when we add scope.
    Expression(int scope) : label(""), scope(scope){};

    // constructor for imbeded expressions
    Expression(int scope, Expression *exp);

    void setLabel(string label);
    string getLabel();

    int getScope(){return scope;}
};

class Variable 
{
private:
    string name;
    int scope;
    void *data;

public:

    // constructor for variables that don't have data yet
    Variable(string name, int scope) : name(name), scope(scope){};

    Variable(string name, int scope, void* data) : name(name), scope(scope), data(data){};

    string getName(){return name;}
    int getScope(){return scope;}

    void* getData(){return data;}
    void setData(void *data);
};

#endif // EXPRESSION_H
