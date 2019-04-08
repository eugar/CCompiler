//
// Created by mmalett on 4/7/19.
//

#ifndef CCOMPILER_INSTRUCTIONS_H
#define CCOMPILER_INSTRUCTIONS_H

#include <vector>
#include <string>

#include "ir.h"

class Statement;

class Function
{
public:

    Function(pnode funcRoot);
    void extractStatements(pnode root);

private:
    std::vector<Statement> m_statementList;

};

// Abstract base class
class Statement
{
public:

    Statement(pnode node)
            : m_root(node)
    {}
    static bool rCompNode(pnode a, pnode b) {return a.rule() >= b.rule();}

    virtual void parseExprStmt(pnode root, Statement &exprStmt);
    virtual void parseSelStmt(pnode root, Statement &selStmt);
    virtual void parseIterStmt(pnode root, Statement &iterStmt);
    virtual void parseRetStmt(pnode root, Statement &retStmt);
    virtual void parseBreakStmt(pnode root, Statement &breakStmt);
    virtual void parseVarDecl(pnode root, Statement &varDecl);
    pnode m_root;
    std::vector<irInstruction> m_instructions;

private:
    void dfsExpr(pnode node, std::pair<std::string, int> &varIter);
    void translateTerm(pnode node, std::pair<std::string, int> &varIter);
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
