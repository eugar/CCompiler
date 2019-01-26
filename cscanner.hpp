
#include "cexpression.hpp"
#include <map>

using namespace std;

class Scanner {
private:
    map<string, int> cinstructions;
    Expression parseLine(string line);
    int findToken(string token);
public:
    Scanner();
    vector<Expression> scanFile(string filename);
};
