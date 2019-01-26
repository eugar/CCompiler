#include "ccompiler.hpp"
#include <iostream>

using namespace std;

/**
 * [Scanner::Scanner scanner constructor]
 * [Sets all the instructions the will change the programs state]
 */
Scanner::Scanner(){
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
}

/**
 * [Scanner::findToken searches for the given token in cinstructions map]
 * @param  token [the token]
 * @return       [the corresponding value in cinstructions map]
 */
int Scanner::findToken(string token){
    map<string, int>::const_iterator pos = cinstructions.find(token);
    int value = -1;
    if (pos == cinstructions.end()) {
        //cout<<"token: "<<token<<" not an instruction. Variable?"<<endl;
    } else {
        value = pos->second;
    }
    return value;
}



/**
 * [Scanner::parseLine parses each line into <token, label>]
 * @param  line [current line in the file being compiled]
 * @return      [returns the expression generated from each line]
 */
Expression Scanner::parseLine(string line){
    Expression expression = Expression();
    string token = "";
    int mapValue = -1;

    //cout << line << endl;
    for(int i = 0; i < line.size(); i++) {
        //cout << line[i];

        if(line[i] != ' ') {
            token += line[i];
        } else if (token != ""){
            mapValue = findToken(token);
            //expression.setToken(token);
            switch (mapValue) {
                case INT:
                    //expression.setLabel("");
                    cout <<"<"<<token<<", integer declaration>\n"<<endl;
                    break;
                case DUB:
                    cout <<"<"<<token<<", double declaration>\n"<<endl;
                    break;
                case SHORT:
                    cout <<"<"<<token<<", short declaration>\n"<<endl;
                    break;
                case FLOAT:
                    cout <<"<"<<token<<", float declaration>\n"<<endl;
                    break;
                case CHAR:
                    cout <<"<"<<token<<", character declaration>\n"<<endl;
                    break;
                case PLUS:
                    cout <<"<"<<token<<", plus symbol>\n"<<endl;
                    break;
                case MINUS:
                    cout <<"<"<<token<<", minus symbol>\n"<<endl;
                    break;
                case MUL:
                    cout <<"<"<<token<<", multiplication>\n"<<endl;
                    break;
                case DIV:
                    cout <<"<"<<token<<", division>\n"<<endl;
                    break;
                case PP:
                    cout <<"<"<<token<<", plus plus>\n"<<endl;
                    break;
                case MM:
                    cout <<"<"<<token<<", minus minus>\n"<<endl;
                    break;
                case PLUSASGN:
                    cout <<"<"<<token<<", plus equals>\n"<<endl;
                    break;
                case MINASGN:
                    cout <<"<"<<token<<", minus equals>\n"<<endl;
                    break;
                case DIVASGN:
                    cout <<"<"<<token<<", division equals>\n"<<endl;
                    break;
                case ASGN:
                    cout <<"<"<<token<<", equals>\n"<<endl;
                    break;
                case GOTO:
                    cout <<"<"<<token<<", terrible programming>\n"<<endl;
                    break;
                case IF:
                    cout <<"<"<<token<<", if statement>\n"<<endl;
                    break;
                case ELSE:
                    cout <<"<"<<token<<", else statement>\n"<<endl;
                    break;
                case NOT:
                    cout <<"<"<<token<<", negation>\n"<<endl;
                    break;
                case RET:
                    cout <<"<"<<token<<", return statement>\n"<<endl;
                    break;
                case BRK:
                    cout <<"<"<<token<<", break statement>\n"<<endl;
                    break;
                case WHILE:
                    cout <<"<"<<token<<", while loop>\n"<<endl;
                    break;
                default:
                    cout <<"<"<<token<<", Not in the set of known instructions>\n"<<endl;
                    break;
            }
            token = "";
        }
    }

    return expression;
}

/**
 * [Scanner::scanFile scans the file and seperates it into expressions]
 * calls parseLine()
 * @param  filename [name of the file]
 * @return          [returns a vector of expressions generated]
 */
vector<Expression> Scanner::scanFile(string filename){
    vector<Expression> expressions;
    fstream file;
    string line;

    file.open(filename);

    if (file.is_open()) {
        while (getline(file, line)) {
            expressions.push_back(parseLine(line));
        }
        file.close();
    } else {
        cout << "File " << filename << " could not be opened" << endl;
    }

    return expressions;
}
