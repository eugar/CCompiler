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
#endif // EXPRESSION_H
