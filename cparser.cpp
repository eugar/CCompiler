#include "cparser.hpp"
#include <iostream>
#include <fstream>

#define DEBUG 1

using namespace std;

Parser::Parser()
{
    loadGrammar();
    //buildTables();
}

void Parser::parseTokens(vector<Token> tokenList)// list of known tokens
{
    int i = 0;

    m_printTree.push_back("[");
    m_printTree.push_back("Program");
    m_printTree.push_back("[");
    m_printTree.push_back("DecList");

    while (!tokenList.empty())
    {
        setState(i, tokenList);
        tokenList.pop_back();
        i++;
    }
    m_printTree.push_back("]");
    m_printTree.push_back("]");
}

// check and changes state
// determined by the list of tokens
void Parser::setState(int i, // the current token
                    vector<Token> &tokenList) // the token list
{
    int state = m_stateList.size()-1;
    int mapValue = tokenList[i].first;

    // Check if the token is an initial state
    switch (mapValue)
    {
        case INT:

        case DUB:

        case SHORT:

        case FLOAT:

        case CHAR:
            m_stateList.push_back(TYPE);
            return;
    }

    // Error check:
    // if token is not a TYPE it cannot be an inital type
    if (state < 0)
    {
        return;
    }

    // States other than TYPES
    switch (mapValue)
    {
        case PLUS:
            break;
        case MINUS:
            break;
        case MUL:
            break;
        case DIV:
            break;
        case PP:
            break;
        case MM:
            break;
        case PLUSASGN:
            break;
        case MINASGN:
            break;
        case MULASGN:
            break;
        case DIVASGN:
            break;
        case ASGN:
            break;
        case GOTO:
            break;
        case IF:
            break;
        case ELSE:
            break;
        case NOT:
            break;
        case RET:
            if (m_stateList[state == STMT]) {
                m_stateList.push_back(RTSTMT);
                m_printTree.push_back("RtStmt");
                m_printTree.push_back("[");
                m_stateList.push_back(RET);
                m_printTree.push_back("[return]");
            }
            break;
        case BRK:
            break;
        case WHILE:
            break;
        case OPAREN:
            if (m_stateList[state] == DECL)
            {
                m_stateList.pop_back();         // pop DECL and
                m_stateList.pop_back();         // pop TYPE
                m_stateList.push_back(FUNC);    // replace with state func
                m_printTree.push_back("FuncDecl");
                m_printTree.push_back("[");
                m_printTree.push_back("Type");
                m_printTree.push_back("[");
                m_printTree.push_back(tokenList[i-2].second);
                m_printTree.push_back("]");
                m_printTree.push_back("]");
                m_printTree.push_back("[");
                m_printTree.push_back("ID");
                m_printTree.push_back("[");
                m_printTree.push_back(tokenList[i-1].second);
                m_printTree.push_back("]");
                m_printTree.push_back("]");
                m_stateList.push_back(PRM);     // and params
                m_printTree.push_back("[");
                m_printTree.push_back("Params");
            }
            break;
        case CPAREN:
            if (m_stateList[state] == PRM)
            {
                m_printTree.push_back("]");
                m_stateList.pop_back();         // no longer in parameters
            }
            break;
        case OBRACE:
            if (m_stateList[state] == FUNC)
            {
                m_stateList.push_back(STMT);
                m_printTree.push_back("[");
                m_printTree.push_back("Stmt");
                m_printTree.push_back("[");
            }
            break;
        case CBRACE:
            m_printTree.push_back("]");
            //m_stateList.pop_back();
            if (m_stateList[state] == STMT) {
                m_stateList.pop_back();
                m_printTree.push_back("]");
            }
            break;
        case OBRACK:
            break;
        case CBRACK:
            break;
        case SEMI:
            if (m_stateList[state] == DECL)
            {
                m_printTree.push_back("[");
                m_printTree.push_back(";");
                m_printTree.push_back("]");
                m_printTree.push_back("]");
                m_stateList.pop_back();         // pop id
                state -= 1;
                // expression done
                while (m_stateList[state] == DECL || m_stateList[state] == TYPE)
                {
                    m_printTree.push_back("]");
                    m_stateList.pop_back();
                    state -= 1;
                    if (state == -1)
                    {
                        break;
                    }
                }
            }
            else if (m_stateList[state] == RTSTMT)
            {
                m_stateList.pop_back();
                m_printTree.push_back("]");
            }
            break;
        case NUMCONST:
            if (m_stateList[state] == RET) {
                m_stateList.pop_back();
                m_stateList.push_back(RTSTMT);
                m_printTree.push_back("[");
                m_printTree.push_back("NumConst");
                m_printTree.push_back("[");
                m_printTree.push_back(tokenList[i].second);
                m_printTree.push_back("]");
                m_printTree.push_back("]");
                m_printTree.push_back("]");
            }
            break;
        case ID:
            if (m_stateList[state] == TYPE)
            {
                m_stateList.push_back(DECL);    //DECL state
                m_printTree.push_back("[");
                m_printTree.push_back("Decleration");
                m_printTree.push_back("[");
            }
            break;
        case LSTHN:
            break;
        case GRTHN:
            break;
        case GRTHEQ:
            break;
        case LSTHEQ:
            break;
        case EQUAL:
            break;
        case AND:
            break;
        case OR:
            break;
        case DQUOT:
            break;
        case SQUOT:
            break;
        default:
            cout << "<" << ", Not in the set of known instructions>\n" << endl;
            break;
    }
}

void Parser::printParseTree()
{
    for(int i=0; i < m_printTree.size(); i++)
    {
        cout << m_printTree[i] << " ";
    }
    cout << endl;
}
void Parser::loadGrammar()
{
    ifstream fs("grammar.txt");
    if (!fs.is_open())
        throw "unable to open file";
    string line;
    while(getline(fs, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        string LHS;
        int start = 0;
        int length = 0;
        while (!isspace(line[start + length]) && line[start + length] != '~')
        {
            length++;
        }
        LHS = line.substr(start, length);
        m_prodRuleIndex.push_back(LHS);

        if (!nextRule(line, start, length))
        {
            throw "Grammar is missing the right hand side";
        }

        vector<string> RHS;
        while (!isspace(line[start + length]))
        {
            while (line[start + length] != '|')
            {
                length++;
            }
            string rule(line.substr(start, length));

            RHS.push_back(rule);

            if (!nextRule(line, start, length))
            {
                break;
            }
        }
        m_pRule[LHS] = RHS;

    }

#ifdef DEBUG
    for (auto lhs : m_prodRuleIndex)
    {
        string RHS;
        auto  RHSvec = m_pRule[lhs];
        int i = 1;
        int j = 0;
        for (auto rhs : RHSvec)
        {
            string rulenum = to_string(i) + string(". ");
            RHS += rulenum + rhs;
            i++;
        }
        cout << "LHS : " << m_prodRuleIndex[j] << "  --->  RHS : " << RHS << endl ;
        j++;
    }
#endif
}
bool Parser::nextRule(string line, int &start, int &length)
{
    start = start + length;
    length = 0;
    auto it = line.begin() + start;
    while (!isalpha(line[start]))
    {
        it++;
        if ( it == line.end())
            return false;
        start++;

    }
    return true;
}
