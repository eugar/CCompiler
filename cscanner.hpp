#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <map>
#include <vector>

using namespace std;

class Scanner 
{

public:
    Scanner();
    void openFile(string filename, vector<Token> &tokenList);

private:
    void scanFile(istream &input, vector<Token> &tokenList);
    tokType findToken(string token);
    map<string, tokType> cinstructions;
};

#endif // SCANNER_H
