
#include <string>

using namespace std;

#define ID 1 //not in cinstructions
#define INT 2
#define DUB 3
#define SHORT 4
#define FLOAT 5
#define CHAR 6
#define FUNC 7 //not in cinstructions
#define KEY 8 //not in cinstructions
#define PLUS 9
#define MINUS 10
#define MUL 11
#define DIV 12
#define PP 13
#define MM 14
#define PLUSASGN 15
#define MINASGN 16
#define MULASGN 17
#define DIVASGN 18
#define ASGN 19
#define BOOL 20 //not in cinstructions
#define GOTO 21
#define IF 22
#define ELSE 23
#define NOT 24
#define RET 25
#define BRK 26
#define WHILE 27

class Expression {
private:
    string token;
    string label;

public:
    Expression() : token(""), label(""){};

    void setToken(string token);
    string getToken();

    void setLabel(string label);
    string getLabel();
};