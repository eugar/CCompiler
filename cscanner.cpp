#include "ccompiler.hpp"
#include <iostream>

using namespace std;

// Sets all the instructions the will change the programs state
Scanner::Scanner()
{
    cinstructions.insert(pair<string, int>("id", ID));
    cinstructions.insert(pair<string, int>("int", INT));
    cinstructions.insert(pair<string, int>("double", DUB));
    cinstructions.insert(pair<string, int>("short", SHORT));
    cinstructions.insert(pair<string, int>("float", FLOAT));
    cinstructions.insert(pair<string, int>("char", CHAR));
    cinstructions.insert(pair<string, int>("+", PLUS));
    cinstructions.insert(pair<string, int>("-", MINUS));
    cinstructions.insert(pair<string, int>("*", MUL));
    cinstructions.insert(pair<string, int>("/", DIV));
    cinstructions.insert(pair<string, int>("++", PP));
    cinstructions.insert(pair<string, int>("--", MM));
    cinstructions.insert(pair<string, int>("+=", PLUSASGN));
    cinstructions.insert(pair<string, int>("-=", MINASGN));
    cinstructions.insert(pair<string, int>("*=", MULASGN));
    cinstructions.insert(pair<string, int>("/=", DIVASGN));
    cinstructions.insert(pair<string, int>("=", ASGN));
    cinstructions.insert(pair<string, int>("goto", GOTO));
    cinstructions.insert(pair<string, int>("if", IF));
    cinstructions.insert(pair<string, int>("else", ELSE));
    cinstructions.insert(pair<string, int>("!", NOT));
    cinstructions.insert(pair<string, int>("return", RET));
    cinstructions.insert(pair<string, int>("break", BRK));
    cinstructions.insert(pair<string, int>("while", WHILE));
    cinstructions.insert(pair<string, int>("(", OPAREN));
    cinstructions.insert(pair<string, int>(")", CPAREN));
    cinstructions.insert(pair<string, int>("{", OBRACE));
    cinstructions.insert(pair<string, int>("}", CBRACE));
    cinstructions.insert(pair<string, int>("[", OBRACK));
    cinstructions.insert(pair<string, int>("]", CBRACK));
    cinstructions.insert(pair<string, int>(";", SEMI));
    cinstructions.insert(pair<string, int>("num", NUMCONST));
    cinstructions.insert(pair<string, int>("!", NOT));
    cinstructions.insert(pair<string, int>("&&", AND));
    cinstructions.insert(pair<string, int>("||", OR));
    cinstructions.insert(pair<string, int>("<", LSTHN));
    cinstructions.insert(pair<string, int>(">", GRTHN));
    cinstructions.insert(pair<string, int>("<=", LSTHEQ));
    cinstructions.insert(pair<string, int>(">=", GRTHEQ));
    cinstructions.insert(pair<string, int>("==", EQUAL));
    cinstructions.insert(pair<string, int>("\"", DQUOT));
    cinstructions.insert(pair<string, int>("\'", SQUOT));
}



 // Searches for the given token in cinstructions map]
 // Returns the corresponding value in cinstructions map
int Scanner::findToken(string token) // The token.
{
    map<string, int>::const_iterator pos = cinstructions.find(token);
    int value = -1;
    if (pos == cinstructions.end()) {
        //cout<<"token: "<<token<<" not an instruction. Variable?"<<endl;
    } 
    else {
        value = pos->second;
    }
    return value;
}

// Prints the token, and passes mapValue back as the return.
int Scanner::printTokens(int mapValue, // The value of the token in cinstructions map
			 string token) // The token.
{
    switch (mapValue) {
        case INT:
            //expression.setLabel("");
            cout << "<" << token << ", integer declaration>\n" << endl;
            break;
        case DUB:
            cout << "<" << token << ", double declaration>\n" << endl;
            break;
        case SHORT:
            cout << "<" << token << ", short declaration>\n" << endl;
            break;
        case FLOAT:
            cout << "<" << token << ", float declaration>\n" << endl;
            break;
        case CHAR:
            cout << "<" << token << ", character declaration>\n" << endl;
            break;
        case PLUS:
            cout << "<" << token << ", plus symbol>\n" << endl;
            break;
        case MINUS:
            cout << "<" << token << ", minus symbol>\n" << endl;
            break;
        case MUL:
            cout << "<" << token << ", multiplication>\n" << endl;
            break;
        case DIV:
            cout << "<" << token << ", division>\n" << endl;
            break;
        case PP:
            cout << "<" << token << ", plus plus>\n" << endl;
            break;
        case MM:
            cout << "<" << token << ", minus minus>\n" << endl;
            break;
        case PLUSASGN:
            cout << "<" << token << ", plus assignment>\n" << endl;
            break;
        case MINASGN:
            cout << "<" << token << ", minus assignment>\n" << endl;
            break;
        case MULASGN:
            cout << "<" << token << ", multiply assignemnt>\n" << endl;
            break;
        case DIVASGN:
            cout << "<" << token << ", division assignment>\n" << endl;
            break;
        case ASGN:
            cout << "<" << token << ", assignment>\n" << endl;
            break;
        case GOTO:
            cout << "<" << token << ", terrible programming>\n" << endl;
            break;
        case IF:
            cout << "<" << token << ", if statement>\n" << endl;
            break;
        case ELSE:
            cout << "<" << token << ", else statement>\n" << endl;
            break;
        case NOT:
            cout << "<" << token << ", negation>\n" << endl;
            break;
        case RET:
            cout << "<" << token << ", return statement>\n" << endl;
            break;
        case BRK:
            cout << "<" << token << ", break statement>\n" << endl;
            break;
        case WHILE:
            cout << "<" << token << ", while loop>\n" << endl;
            break;
        case OPAREN:
            cout << "<" << token << ", opening parenthesis>\n" << endl;
            break;
        case CPAREN:
            cout << "<" << token << ", closing parenthesis>\n" << endl;
            break;
        case OBRACE:
            cout << "<" << token << ", opening brace>\n" << endl;
            break;
        case CBRACE:
            cout << "<" << token << ", closing brace>\n" << endl;
            break;
        case OBRACK:
            cout << "<" << token << ", opening bracket>\n" << endl;
            break;
        case CBRACK:
            cout << "<" << token << ", closing bracket>\n" << endl;
            break;
        case SEMI:
            cout << "<" << token << ", semicolon>\n" << endl;
            break;
        case NUMCONST:
            cout << "<" << token << ", number constant>\n" << endl;
            break;
        case ID:
            cout << "<" << token << ", identifier>\n" << endl;
            break;
        case LSTHN:
            cout << "<" << token << ", less than>\n" << endl;
            break;
        case GRTHN:
            cout << "<" << token << ", greather than>\n" << endl;
            break;
        case GRTHEQ:
            cout << "<" << token << ", greather than or equal to>\n" << endl;
            break;
        case LSTHEQ:
            cout << "<" << token << ", less than or equal to>\n" << endl;
            break;
        case EQUAL:
            cout << "<" << token << ", equals>\n" << endl;
            break;
        case AND:
            cout << "<" << token << ", and>\n" << endl;
            break;
        case OR:
            cout << "<" << token << ", or>\n" << endl;
            break;
        case DQUOT:
            cout << "<" << token << ", double quote>\n" << endl;
            break;
        case SQUOT:
            cout << "<" << token << ", single quote>\n" << endl;
            break;
        default:
            cout << "<" << token << ", Not in the set of known instructions>\n" << endl;
            break;
    }
    return mapValue;
}

// Tokenizes the input file stream.
void Scanner::parseFile(istream &input, // [in] file stream to tokenize
			vector<Token> &tokenList) // [out] List to populate with tokens. 
{
    tokenList.clear();
    string token;
    token.clear();
    int mapValue = -1;

    while (true)
    {
        char c = input.get();
        if (!input.good())
        {
            break;
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
                if (isalnum(c) || c == '_')
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
                    tokenList.push_back(Token(findToken(token), token));

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
                        tokenList.push_back(Token(findToken(token), token));
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

                if ((mapValue = findToken(token)) > 0) 
                {
                    // keyword
                    tokenList.push_back(Token(mapValue, token));
                }
                else 
                {
                    // id or numconstant
                    if (isdigit(token[0]))
                    {
                        // define as num constant and check integrity later
                        tokenList.push_back(Token(NUMCONST, token));
                    }
                    else 
                    {
                        // define as identifier and check integrity later
                        tokenList.push_back(Token(ID, token));
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
void Scanner::scanFile(string filename, // Path of file to program
		       vector<Token> &tokenList) // List to populate with tokens.
{
    fstream file;
    string line;

    file.open(filename);

    if (file.is_open()) 
    {
        parseFile(file, tokenList);
        file.close();
    } 
    else 
    {
        cout << "File " << filename << " could not be opened" << endl;
    }
}
