//
// Created by mmalett on 4/7/19.
//
// These classes are intended to manipulate the parse tree into a three
// address code intermediate representation. Since a function is the smallest
// unit that can comprise a program it starts there. A statement is what
// a function is made of. So this is intended to systematically go through the
// legal grammar statements (that are stored in the parsetree) and break them
// into pieces that can logically be redefined into the IR format.

#ifndef CCOMPILER_INSTRUCTIONS_H
#define CCOMPILER_INSTRUCTIONS_H

#include <vector>
#include <list>
#include <string>
#include "cparser.hpp"

//#include "ir.h"

typedef struct irInstruction
{
    // default constructor that zeroes out all values.
    irInstruction()
    : op()
    , arg1()
    , params()
    , arg2()
    , res()
    {}

    string op;
    string arg1;
    pnode *params;
    string arg2;
    string res;
} irInstruction;

class Statement;

class Function
{
public:

    Function(pnode funcRoot);
    void extractStatements(pnode root);
    std::vector<Statement> getStatementList(){return m_statementList;}

private:
    std::vector<Statement> m_statementList;

};

// Abstract base class
class Statement
{
public:

    Statement(pnode node)
            : m_root(node)
    {
        dfsStmt(m_root);
    }
    static bool rCompNode(pnode a, pnode b) {return a.rule() >= b.rule();}
    std::vector<irInstruction> getCurTerms(){return m_curTerms;}

    virtual void parseExprStmt(pnode root, Statement &exprStmt);
    virtual void parseSelStmt(pnode root, Statement &selStmt);
    virtual void parseIterStmt(pnode root, Statement &iterStmt);
    virtual void parseRetStmt(pnode root, Statement &retStmt);
    virtual void parseBreakStmt(pnode root, Statement &breakStmt);
    virtual void parseVarDecl(pnode root, Statement &varDecl);

    pnode m_root;
    std::vector<Statement> m_statements;
    std::list<irInstruction> m_instructions;

private:
    void dfsStmt(pnode node);
    void dfsCompStmt(pnode node, std::pair<std::string, int> &varIter);
    void dfsExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsSimpleExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsAndExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsUnaryRelExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsRelExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsSumExpr(pnode node, std::pair<std::string, int> &varIter);
    void dfsTerm(pnode node, std::pair<std::string, int> &varIter);
    void dfsUnaryExpr(pnode node, std::pair<string, int> &varIter);
    void dfsImmutable(pnode node, std::pair<string, int> &varIter);
    void dfsConstant(pnode node, std::pair<string, int> &varIter);
    void dfsCall(pnode node, std::pair<string, int> &varIter);
    void dfsIfStmt(pnode node, std::pair<string, int> &varIter);
    void dfsElifStmt(pnode node, std::pair<string, int> &varIter);
    void dfsElseStmt(pnode node, std::pair<string, int> &varIter);
    void dfsArgs(pnode node, std::pair<string, int> &varIter);
    void dfsArgList(pnode node, std::pair<string, int> &varIter);
    void dfsVarDeclList(pnode node, std::pair<string, int> &varIter);
    void dfsVarDeclInit(pnode node, std::pair<string, int> &varIter);

    void translateTerm(pnode node, std::pair<std::string, int> &varIter);

    void processMutUnaryOp(pnode node, std::pair<std::string, int> &varIter);


    std::vector<irInstruction> m_curTerms;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(pnode node)
    : Statement(node)
    {
        parseExprStmt(node, *this);
    }
    ~ExpressionStatement() = default;

};

class SelectionStatement : public Statement
{
public:
    SelectionStatement(pnode node)
    : Statement(node)
    {
        parseSelStmt(node, *this);
    }
    ~SelectionStatement() = default;

};

class IterationStatement : public Statement
{
public:
    IterationStatement(pnode node)
    : Statement(node)
    {
        parseIterStmt(node, *this);
    }
    ~IterationStatement() = default;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement(pnode node)
    : Statement(node)
    {
        parseRetStmt(node, *this);
    }
    ~ReturnStatement() = default;
};

class BreakStatement : public Statement
{
public:
    BreakStatement(pnode node)
    : Statement(node)
    {
        parseBreakStmt(node, *this);
    }
    ~BreakStatement() = default;
};

class VariableDeclaration : public Statement
{
public:
    VariableDeclaration(pnode node)
    : Statement(node)
    {
        parseVarDecl(node, *this);
    }
    ~VariableDeclaration() = default;
};

#endif //CCOMPILER_INSTRUCTIONS_H
