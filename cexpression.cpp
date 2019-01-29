#include "cexpression.hpp"

using namespace std;

// Expression == <token, label>

/**
 * [Expression::Expression constructor for embeded Expression]
 * @param scope [the scope]
 * @param exp   [the embeded expression]
 */
Expression::Expression(int scope, Expression *exp)
{
    label = "";
    this->scope = scope;
    this->exp = exp;
}

/**
 * [Expression::setLabel sets the label of the expression]
 * @param label [the label]
 */
void Expression::setLabel(string label)
{
    label = label;
}

/**
 * [Expression::getLabel gets the label of the expression]
 * @return [the label]
 */
string Expression::getLabel()
{
    return label;
}

/**
 * [Variable::setData sets the data that the variable holds]
 * @param data [The new data]
 */
void Variable::setData(void* data)
{
    this->data = data;
}
