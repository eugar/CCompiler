
#include "cscanner.hpp"
#include <vector>
#include <fstream>

using namespace std;

class Compiler {
private:
    Scanner scanner;
    vector<Expression> expressions;

public:
    Compiler();
    void compile(string filename);
    vector<Expression> getExpressions();
};
