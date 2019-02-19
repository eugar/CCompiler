#ifndef SCANNER_H
#define SCANNER_H

#include "cexpression.hpp"
#include <map>
#include <vector>

using namespace std;

class Scanner 
{

public:
    Scanner();
    void scanFile(string filename, vector<Tok> &tokenList);
    int printTokens(int mapValue, string token);

private:
    map<string, int> cinstructions;
    void parseFile(istream &input, vector<Tok> &tokenList);
    int findToken(string token);

};

#endif // SCANNER_H
