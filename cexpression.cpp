#include "cexpression.hpp"

using namespace std;

// Expression == <token, label>

/**
 * [Expression::setToken sets the token of the expression]
 * @param token [the token]
 */
void Expression::setToken(string token){
    token = token;
}

/**
 * [Expression::getToken gets the token of the expression]
 * @return [the token]
 */
string Expression::getToken(){
    return token;
}

/**
 * [Expression::setLabel sets the label of the expression]
 * @param label [the label]
 */
void Expression::setLabel(string label){
    label = label;
}

/**
 * [Expression::getLabel gets the label of the expression]
 * @return [the label]
 */
string Expression::getLabel(){
    return label;
}

/**
 * [Variable::setData sets the data that the variable holds]
 * @param data [The new data]
 */
void Variable::setData(void* data){
    this->data = data;
}
