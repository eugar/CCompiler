
#include "cscanner.hpp"
#include <vector>
#include <fstream>

using namespace std;

class Compiler {
private:
    Scanner scanner;
    vector<Expression> expressions;
    vector<Variable> variables;

public:
    Compiler();
    void compile(string filename);
    vector<Expression> getExpressions();

    vector<Variable> getVariables();
    void addVariable(Variable var);
};
