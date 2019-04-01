//
// Created by Victor Gutierrez
//
#include "ccompiler.hpp"
#include "cscanner.hpp"
#include <iostream>

using namespace std;

// Sets all the instructions the will change the programs state
Scanner::Scanner()
{
    cinstructions.insert(pair<string, tokType>("id", ID));
    cinstructions.insert(pair<string, tokType>("int", INT));
    cinstructions.insert(pair<string, tokType>("double", DUB));
    cinstructions.insert(pair<string, tokType>("short", SHORT));
    cinstructions.insert(pair<string, tokType>("float", FLOAT));
    cinstructions.insert(pair<string, tokType>("char", CHAR));
    cinstructions.insert(pair<string, tokType>("+", PLUS));
    cinstructions.insert(pair<string, tokType>("-", MINUS));
    cinstructions.insert(pair<string, tokType>("*", MUL));
    cinstructions.insert(pair<string, tokType>("/", DIV));
    cinstructions.insert(pair<string, tokType>("++", PP));
    cinstructions.insert(pair<string, tokType>("--", MM));
    cinstructions.insert(pair<string, tokType>("+=", PLUSASGN));
    cinstructions.insert(pair<string, tokType>("-=", MINASGN));
    cinstructions.insert(pair<string, tokType>("*=", MULASGN));
    cinstructions.insert(pair<string, tokType>("/=", DIVASGN));
    cinstructions.insert(pair<string, tokType>("=", ASGN));
    cinstructions.insert(pair<string, tokType>("goto", GOTO));
    cinstructions.insert(pair<string, tokType>("if", IF));
    cinstructions.insert(pair<string, tokType>("else", ELSE));
    cinstructions.insert(pair<string, tokType>("!", NOT));
    cinstructions.insert(pair<string, tokType>("return", RET));
    cinstructions.insert(pair<string, tokType>("break", BRK));
    cinstructions.insert(pair<string, tokType>("while", WHILE));
    cinstructions.insert(pair<string, tokType>("(", OPAREN));
    cinstructions.insert(pair<string, tokType>(")", CPAREN));
    cinstructions.insert(pair<string, tokType>("{", OBRACE));
    cinstructions.insert(pair<string, tokType>("}", CBRACE));
    cinstructions.insert(pair<string, tokType>("[", OBRACK));
    cinstructions.insert(pair<string, tokType>("]", CBRACK));
    cinstructions.insert(pair<string, tokType>(";", SEMI));
    cinstructions.insert(pair<string, tokType>("num", NUMCONST));
    cinstructions.insert(pair<string, tokType>("!", NOT));
    cinstructions.insert(pair<string, tokType>("&&", AND));
    cinstructions.insert(pair<string, tokType>("||", OR));
    cinstructions.insert(pair<string, tokType>("<", LSTHN));
    cinstructions.insert(pair<string, tokType>(">", GRTHN));
    cinstructions.insert(pair<string, tokType>("<=", LSTHEQ));
    cinstructions.insert(pair<string, tokType>(">=", GRTHEQ));
    cinstructions.insert(pair<string, tokType>("==", EQUAL));
    cinstructions.insert(pair<string, tokType>("!=", NOTEQ));
    cinstructions.insert(pair<string, tokType>("\"", DQUOT));
    cinstructions.insert(pair<string, tokType>("\'", SQUOT));
}


// Tokenizes the input file stream.
void Scanner::scanFile(istream &input, // [in] file stream to tokenize
			vector<Token> &tokenList) // [out] List to populate with tokens.
{
    tokenList.clear();
    string token;
    token.clear();
    int mapValue = -1;
    int lineNo = 1;

    while (true)
    {
        char c = input.get();
        if (!input.good())
        {
            break;
        }

        if (c == '\n')
        {
            lineNo++;
            continue;
        }
        // Look for next token
        if (token.empty())
        {
            if (isspace(c))
            {
                continue;
            }
            else
            {

                if (c == '/' && ((char)input.peek() == '*' || (char)input.peek() == '/'))
                {
                    c = input.get();

                    if (!input.good())
                    {
                        break;
                    }

                    if (c == '*')
                    {
                        int bComm = 1;
                        c = input.get();
                        //cout << "found a block comment" << endl;
                        tokenList.push_back(Token("/*", BCOMM, lineNo));
                        while (bComm)
                        {
                            c = input.get();
                            if (!input.good())
                            {
                                break;
                            }

                            if (c == '\n')
                            {
                                lineNo++;
                            }

                            if (c == '*' && (char)input.peek() == '/')
                            {
                                bComm = 0;
                                c = input.get();
                            }
                        }
                    }

                    else if (c == '/')
                    {
                        //cout << "found a single (line) comment" << endl;
                        tokenList.push_back(Token("//", SCOMM, lineNo));
                        while (c != '\n')
                        {
                            c = input.get();
                            if (!input.good())
                            {
                                break;
                            }
                        }
                        lineNo++;
                    }
                }

                else if (isalnum(c) || c == '_')
                {
                    token.append(1, c);
                    continue;
                }
                // single characters that require no further checks
                else if (c == '(' || c == ')' || c == '\'' || c == '\"' ||
                         c == ';' || c == '{' || c == '}' || c == '[' ||
                         c == ']' || c == '#')
                {
                    token.append(1, c);
                    tokenList.push_back(Token(token, findToken(token), lineNo));

                    token.clear();

                    // Handle strings and chars starting here. Checks for closing
                    // parens/braces/brackets also would be here although we'd want to
                    // separate out the ; and # from this IF clause should
                    // we implement that.
                }
                // ops that could be 1 or 2 chars long
                else if (c == '+' || c == '-' || c == '*' || c == '/' ||
                         c == '=' || c == '&' || c == '|' || c == '!' ||
                         c == '<' || c == '>')
                {
                    token.append(1, c);
                    char next = input.peek();
                    if ((next == '=') || (next == c && (c != '*' && c != '/' && c != '!')))
                    {
                        token.append(1, input.get());
                    }
                        tokenList.push_back(Token(token, findToken(token), lineNo));
                        token.clear();

                }
            }

        }
        // have a piece of a keyword/id/digit, find the rest
        else
        {
            if (c == '(' || c == ')' || c == '\'' || c == '\"' ||
                c == ';' || c == '+' || c == '-' || c == '*' ||
                c == '/' || c == '=' || c == '&' || c == '|' ||
                c == '!' || c == '<' || c == '>' || isspace(c))
            {

                if ((findToken(token)) != ERR)
                {
                    // keyword
                    tokenList.push_back(Token(token, findToken(token), lineNo));
                }
                else
                {
                    // id or numconstant
                    if (isdigit(token[0]))
                    {
                        if (token.find_first_not_of(".0123456789") != std::string::npos)
                        {
                            cout << "invalid number constant: " << token << endl;
                            exit(EXIT_FAILURE);
                        }
                        // define as num constant and check integrity later
                        tokenList.push_back(Token(token, NUMCONST, lineNo));
                    }
                    else
                    {
                        // define as identifier and check integrity later
                        tokenList.push_back(Token(token, ID, lineNo));
                    }
                }
                token.clear();
                input.unget();
                continue;
            }
            else
            {
                token.append(1, c);
            }
        }
    }
}

// Reads in the file and calls parseFile to tokenize it.
void Scanner::openFile(string filename, // Path of file to program
		       vector<Token> &tokenList) // List to populate with tokens.
{
    fstream file;
    string line;

    file.open(filename);

    if (file.is_open())
    {
        scanFile(file, tokenList);
        file.close();
    }
    else
    {
        cout << "File " << filename << " could not be opened" << endl;
    }
}

// used to connect strings and Tokens
tokType Scanner::findToken(string token)
{
map<string, tokType>::const_iterator pos = Scanner::cinstructions.find(token);
tokType value = ERR;
if (pos == Scanner::cinstructions.end()) {
//cout<<"token: "<<token<<" not an instruction. Variable?"<<endl;
}
else {
value = pos->second;
}
return value;

}
