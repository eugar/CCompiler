#include "cexpression.hpp"

using namespace std;

// Expression == <token, label>


// constructor for embeded Expression
Expression::Expression(int scope, // The scope of the expression
		       Expression *exp) // The embedded expression
{
    m_label = "";
    m_scope = scope;
    m_expression = exp;
}
