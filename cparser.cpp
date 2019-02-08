#include "cparser.hpp"
#include <iostream>

using namespace std;

Parser::Parser(){}

void Parser::parseTokens(vector<Token> tokenList)// list of known tokens
{
    int i = 0;

    while (!tokenList.empty()) 
    {
        setState(i, tokenList);
        tokenList.pop_back();
        i++;
    }

    printParseTree();
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

            cout << "Leaf: type -> "<< tokenList[i].second << endl;
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
                cout << "Branch: returnStmt\n";
                m_stateList.push_back(RET);
                cout << "Leaf: return\n";
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

                cout << "Branch: func" << endl;
                m_stateList.push_back(PRM);     // and params
                                cout << "Branch: param" << endl;
            }
            break;
        case CPAREN:
            if (m_stateList[state] == PRM) 
            {
                m_stateList.pop_back();         // no longer in parameters
            }
            break;
        case OBRACE:
            if (m_stateList[state] == FUNC) 
            {
                m_stateList.push_back(STMT);

                cout << "Branch: stmt" << endl;
            }
            break;
        case CBRACE:
            break;
        case OBRACK:
            break;
        case CBRACK:
            break;
        case SEMI:
            if (m_stateList[state] == DECL) 
            {
                m_stateList.pop_back();         // pop id
                state -= 1;
                // expression done
                while (m_stateList[state] == DECL || m_stateList[state] == TYPE) 
                {
                    m_stateList.pop_back();
                    state -= 1;
                    if (state == -1) 
                    {
                        break;
                    }
                }
            }
            break;
        case NUMCONST:
            if (m_stateList[state] == RET) {
                m_stateList.pop_back();
                m_stateList.push_back(NUMCONST);
                cout << "Leaf: NUMCONST -> "<< tokenList[i].second << endl;
            }
            break;
        case ID:
            if (m_stateList[state] == TYPE) 
            {
                m_stateList.push_back(DECL);    //DECL state
<<<<<<< HEAD

                cout << "Branch: decleration -> "<< tokenList[i].second << endl;
=======
                cout << "state: declaration -> "<< tokenList[i].second << endl;
>>>>>>> b0109d65f0cb4882b74edc6f79010a9ce52cd56c
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

}
