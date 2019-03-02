#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

using namespace std;

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
