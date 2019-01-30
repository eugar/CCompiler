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



// constructor for variables that don't have data yet
Variable::Variable(string name, // Variable identifier
		   int scope) // Scope of variable
  :
  m_name(name),
  m_scope(scope)
{};

Variable::Variable(string name, // Variable identifier
		   int scope, // Scope of variable
		   void* data) // Data attributed to variable
  :
  m_name(name),
  m_scope(scope),
  m_data(data)
{};
