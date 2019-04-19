//
// Created by mmalett on 4/7/19.
//

#include "instructions.h"

#include <algorithm>
#include <iostream>

using namespace std;

Function::Function(pnode funcRoot, SymbolTable symbolTable)
{
    this->m_symbolTable = symbolTable;
    pnode compStmtRoot = funcRoot.children()[5].children()[1];
    // sends the root statement list of the compound statement.
    extractStatements(compStmtRoot);
}
// takes the root node of a statement list
void Function::extractStatements(pnode root)
{
    for (auto statement : root.children())
    {
        if (statement.rule() == "stmtList")
        {
            extractStatements(statement);
        }
        else if (statement.rule() == "stmt")
        {
            Statement stmt(statement, m_symbolTable);
            m_statementList.push_back(stmt);
         }
    }
}

void Statement::parseBreakStmt(pnode root, Statement &breakStmt)
{
    // this could be tricky?
}

void Statement::parseSelStmt(pnode root, Statement &selStmt)
{
    for (auto child : root.children())
    {
        if (child.rule() == "ifStmt")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsIfStmt(child, varIter);
        }
        else if (child.rule() == "elseStmt")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsElseStmt(child, varIter);
        }
    }
}

void Statement::dfsStmt(pnode node)
{
    if (node.children()[0].rule() == "exprStmt")
    {
        m_statements.push_back(ExpressionStatement(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "selStmt")
    {
        m_statements.push_back(SelectionStatement(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "iterStmt")
    {
        m_statements.push_back(IterationStatement(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "retStmt")
    {
        m_statements.push_back(ReturnStatement(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "breakStmt")
    {
        m_statements.push_back(BreakStatement(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "varDecl")
    {
        m_statements.push_back(VariableDeclaration(node.children()[0], m_symbolTable));
    }
}

void Statement::dfsIfStmt(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "if")
        {

        }
        else if (child.rule() == "(" || child.rule() == ")")
        {
            // discard symbols
        }
        else if (child.rule() == "simpleExpr")
        {
            dfsSimpleExpr(child, varIter);
        }
        else if (child.rule() == "stmt")
        {
            dfsStmt(child);
        }
        else if (child.rule() == "compStmt")
        {
            dfsCompStmt(child, varIter);
        }
    }
}

void Statement::dfsElseStmt(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "else")
        {
            // discard symbol and set the appropriate behavior.
        }
        else if (child.rule() == "stmt")
        {
            dfsStmt(child);
        }
        else if (child.rule() == "compStmt")
        {
            dfsCompStmt(child, varIter);
        }
        else
        {
            dfsElseStmt(child, varIter);
        }
    }
}

void Statement::dfsCompStmt(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "{" || child.rule() == "}")
        {
            // discard symbol
        }
        else
        {
            dfsStmt(child);
        }
    }
}
void Statement::parseExprStmt(pnode root, Statement &exprStmt)
{
    for (auto child : root.children())
    {
        if (child.rule() == ";")
        {
            return;
        }
        if (child.rule() == "expr")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsExpr(child, varIter);
        }
    }
}

void Statement::parseIterStmt(pnode root, Statement &iterStmt)
{
    for (auto child : root.children())
    {
        if (child.rule() == "while")
        {
            // note that we need to make a loop with what follows.
        }
        else if (child.rule() == "(" || child.rule() == ")")
        {
            // ignore symbols
        }
        else if (child.rule() == "stmt" || child.rule() == "compStmt")
        {
            dfsStmt(child);
        }
    }
}

void Statement::parseRetStmt(pnode root, Statement &retStmt)
{
    for (auto child : root.children())
    {
        if (child.rule() == "return")
        {
            // note a return statement will follow
        }
        else if (child.rule() == ";")
        {
            return;
        }
        else
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsExpr(child, varIter);
        }
    }
}
void Statement::parseVarDecl(pnode root, Statement &varDecl)
{
    for (auto child : root.children())
    {
        if (child.rule() == "typeSpec")
        {
            // currently only goes to return type spec and that goes to terminals
            // so assign whatever terminals
        }
        else if (child.rule() == "varDeclList")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsVarDeclList(child, varIter);
        }
    }
}

void Statement::dfsExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        // the left hand side of an expression.
        if (child.rule() == "mutable")
        {
            // Use symbol table to place value of variable into expression.
            string mutName = child.children()[0].children()[0].rule();
            //auto symbol = SymbolTable::symbolTable->lookup(mutName);

            if (m_curTerms.back().arg1.empty())
            {
                m_curTerms.back().arg1 = "dfsExpr";//= symbol->data;
            }
            else
            {
                m_curTerms.back().arg2 = "dfsExpr";//= symbol->data;
            }

        }
        else if (child.rule() == "mutUnaryOp")
        {
            // manage op
            processMutUnaryOp(child, varIter);
        }
        else if (child.rule() == "mutBinOp")
        {

        }
        else // simpleExpr
        {
            dfsSimpleExpr(node, varIter);
        }
    }
}

void Statement::processMutUnaryOp(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        //auto symbol = SymbolTable::symbolTable->lookup(child.rule());
        if (child.rule() == "++")
        {
            // postfix
            if (node.parent()->children()[0].rule() == "mutable")
            {

            }
            // prefix
            else
            {

            }
        }
        else if (child.rule() == "--")
        {
            // postfix
            if (node.parent()->children()[0].rule() == "mutable")
            {

            }
            // prefix
            else
            {

            }
        }
    }
}

void Statement::dfsSimpleExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "||")
        {
             // manage or op

        }
        else if (child.rule() == "andExpr")
        {
            dfsAndExpr(child, varIter);
        }
        else // simpleExpr
        {
            dfsSimpleExpr(child, varIter);
        }
    }
}

void Statement::dfsAndExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "&&")
        {
            // manage and op
        }
        else if (child.rule() == "unaryRelExpr")
        {
            dfsUnaryRelExpr(child, varIter);
        }
        else
        {
            dfsAndExpr(child, varIter);
        }
    }
}

void Statement::dfsUnaryRelExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "!")
        {
            // manage not op
        }
        else if (child.rule() == "relExpr")
        {
            dfsRelExpr(child, varIter);
        }
        else
        {
            dfsUnaryRelExpr(child, varIter);
        }
    }
}

void Statement::dfsRelExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "relOp")
        {
            // manage relOps
        }
        else
        {
            dfsSumExpr(child, varIter);
        }
    }
}

void Statement::dfsSumExpr(pnode node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "sumOp")
        {
            // manage sumOps
        }
        else if (child.rule() == "term")
        {
            dfsTerm(child, varIter);
        }
        else
        {
            dfsSumExpr(child, varIter);
        }
    }
}

void Statement::dfsTerm(pnode node, std::pair<std::string, int> &varIter)
{

    for (auto child : node.children())
    {
        if (child.rule() == "mulOp")
        {
            // manage mulOps
        }
        else if (child.rule() == "unaryExpr")
        {
            dfsUnaryExpr(child, varIter);
        }
        else
        {
            dfsTerm(child, varIter);
        }
    }
}

void Statement::dfsUnaryExpr(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "mutable")
        {
            // manage mutable, get value from symbol table
        }
        else if (child.rule() == "immutable")
        {
            dfsImmutable(child, varIter);
        }
        else if (child.rule() == "unaryOp")
        {
            // manage unaryOps
        }

    }
}

void Statement::dfsImmutable(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "(" || child.rule() == ")")
        {
            // throw away symbol.
            continue;
        }
        else if (child.rule() == "call")
        {
            dfsCall(child, varIter);
        }
        else if (child.rule() == "constant")
        {
            // manage constant
            dfsConstant(child, varIter);
        }
        else
        {
            dfsExpr(child, varIter);
        }
    }
}

void Statement::dfsConstant(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        irInstruction inst;
        inst.op = "COPY";
        inst.res = "term" + to_string(++(varIter.second));
        m_curTerms.push_back(inst);
        if (child.rule () == "NUMCONST")
        {
            if (m_curTerms.back().arg1.empty())
            {
                m_curTerms.back().arg1 = child.children()[0].rule();
            }
            else
            {
                m_curTerms.back().arg2 = child.children()[0].rule();
            }
        }
        else if (child.rule() == "CHARCONST")
        {
            if (m_curTerms.back().arg1.empty())
            {
                m_curTerms.back().arg1 = child.children()[0].rule();
            }
            else
            {
                m_curTerms.back().arg2 = child.children()[0].rule();
            }
        }
        else if (child.rule() == "true")
        {
            if (m_curTerms.back().arg1.empty())
            {
                m_curTerms.back().arg1 = child.rule();
            }
            else
            {
                m_curTerms.back().arg2 = child.rule();
            }
        }
        else if (child.rule() == "false")
        {
            if (m_curTerms.back().arg1.empty())
            {
                m_curTerms.back().arg1 = child.rule();
            }
            else
            {
                m_curTerms.back().arg2 = child.rule();
            }
        }
    }
}

void Statement::dfsCall(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "ID")
        {
            // manage variable name
            m_curTerms.back().op = child.children()[0].rule();
        }
        else if (child.rule() == "args")
        {
            dfsArgs(child, varIter);
        }
    }
}

void Statement::dfsArgs(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "EMPTY")
        {
            // done here
            m_curTerms.back().params = nullptr;
        }
        else
        {
            irInstruction pushOp;
            m_curTerms.push_back(pushOp);
            dfsArgList(child, varIter);
        }
    }
}

void Statement::dfsArgList(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == ",")
        {
            // throw away symbol
            irInstruction pushOp;
            m_curTerms.push_back(pushOp);
        }
        else if (child.rule() == "expr")
        {

            m_curTerms.back().op = "push";
            dfsExpr(node, varIter);
        }
        else
        {
            dfsArgList(child, varIter);
        }
    }
}

void Statement::dfsVarDeclList(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "varDeclInit")
        {
            dfsVarDeclInit(child, varIter);
        }
        else if (child.rule() == ",")
        {
            // discard symbol
        }
        else
        {
            dfsVarDeclList(node, varIter);
        }
    }
}

void Statement::dfsVarDeclInit(pnode node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "ID")
        {
            // get id
            irInstruction newVarDecl;
            newVarDecl.res = varIter.first + to_string(++(varIter.second));
            m_curTerms.push_back(newVarDecl);
        }
        else if (child.rule() == "=")
        {
            // Discard symbol
        }
        else
        {
            dfsSimpleExpr(child, varIter);
        }
    }
}




void Statement::translateTerm(pnode node, std::pair<std::string, int> &varIter)
{
    string var = varIter.first;
    int i = varIter.second++;


}
