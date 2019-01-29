#ifndef SCANNER_H
#define SCANNER_H

#include "cexpression.hpp"
#include <map>
#include <vector>

using namespace std;

class Scanner {
private:
    map<string, int> cinstructions;
    void parseFile(istream &input, vector<Token> &tokenList);
    int findToken(string token);
public:
    Scanner();
    void scanFile(string filename, vector<Token> &tokenList);
    int printTokens(int mapValue, string token);
};

#endif // SCANNER_H
