//
// Created by mmalett on 4/7/19.
//

#include "instructions.h"

#include <algorithm>
#include <iostream>

using namespace std;

Function::Function(pnode funcRoot, SymbolTable symbolTable)
{
    // ugly but it works ¯\_(ツ)_/¯
    m_funcHeader.op = "FUNC";
    m_funcHeader.res = funcRoot.children()[1].children()[0].rule();
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
            irInstruction newTerm;
            dfsSimpleExpr(child, varIter, newTerm);
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
    irInstruction expr;
    for (auto child : node.children())
    {
        // the left hand side of an expression.
        if (child.rule() == "mutable")
        {
            // Use symbol table to place value of variable into expression.
            string mutName = child.children()[0].children()[0].rule();
            //auto symbol = SymbolTable::symbolTable->lookup(mutName);

            if (expr.arg1.empty())
            {
                //m_curTerms.back().arg1 = "dfsExpr";//= symbol->data;
                expr.res = child.children()[0].rule();
            }
            else
            {
                expr.res = child.children()[0].rule();//= symbol->data;
            }

        }
        // not supported
        else if (child.rule() == "mutUnaryOp")
        {
            // manage op
            processMutUnaryOp(child, varIter, expr);
        }
        else if (child.rule() == "mutBinOp")
        {
            expr.op = child.children()[0].rule();
        }
        else // simpleExpr
        {

            irInstruction newTerm;
            dfsSimpleExpr(node, varIter, newTerm);
        }
    }
    // get result of the last term placed in
    if (!m_curTerms.empty())
    {
        expr.arg1 = m_curTerms.back().res;
        m_curTerms.push_back(expr);
    }
}

void Statement::processMutUnaryOp(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
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

void Statement::dfsSimpleExpr(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    if (node.children().size() > 1)
    {
        dfsAndExpr(node.children()[node.children().size() - 1], varIter, term);
    }
    /*
    for (auto child : node.children())
    {
        if (child.rule() == "||")
        {
            // manage or op
            term.op = "||";
        }
        else if (child.rule() == "andExpr")
        {
            dfsAndExpr(child, varIter, term);
        }
        else // simpleExpr
        {
            irInstruction newTerm;
            dfsSimpleExpr(child, varIter, newTerm);
        }
    }
    */
}

void Statement::dfsAndExpr(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    if (node.children().size() > 1)
    {

    }
    for (auto child : node.children())
    {
        if (child.rule() == "&&")
        {
            // manage and op
            term.op = "&&";
        }
        else if (child.rule() == "unaryRelExpr")
        {
            dfsUnaryRelExpr(child, varIter, term);
        }
        else
        {
            dfsAndExpr(child, varIter, term);
        }
    }
}

void Statement::dfsUnaryRelExpr(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        if (child.rule() == "!")
        {
            // manage not op
            term.op = "!";
        }
        else if (child.rule() == "relExpr")
        {
            dfsRelExpr(child, varIter, term);
        }
        else
        {
            dfsUnaryRelExpr(child, varIter, term);
        }
    }
}

void Statement::dfsRelExpr(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        if (child.rule() == "relOp")
        {
            // manage relOps
            term.op = child.children()[0].rule();
        }
        else
        {
            dfsSumExpr(child, varIter, term);
        }
    }
}

void Statement::dfsSumExpr(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    irInstruction newTerm;
    for (auto child : node.children())
    {
        if (child.rule() == "sumOp")
        {
            // manage sumOps
            term.op = child.children()[0].rule();
        }
        else if (child.rule() == "term")
        {
            dfsTerm(child, varIter, newTerm);
        }
        else
        {
            dfsSumExpr(child, varIter, term);
        }
        if (!m_curTerms.empty())
        {
            term.arg2 = m_curTerms.back().res;
        }
    }
}

void Statement::dfsTerm(pnode node, std::pair<std::string, int> &varIter, irInstruction &term)
{

    for (auto child : node.children())
    {
        if (child.rule() == "mulOp")
        {
            // manage mulOps
            term.op = child.children()[0].rule();
        }
        else if (child.rule() == "unaryExpr")
        {
            dfsUnaryExpr(child, varIter, term);
        }
        else
        {
            irInstruction newTerm;
            dfsTerm(child, varIter, newTerm);
            m_curTerms.push_back(newTerm);
        }
    }
    /*
    if (!m_curTerms.empty())
    {
        term.arg2 = m_curTerms.back().res;
    }
     */
}

void Statement::dfsUnaryExpr(pnode node, std::pair<string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        if (child.rule() == "mutable")
        {
            // manage mutable, get value from symbol table
            if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
            }
        }
        else if (child.rule() == "immutable")
        {
            dfsImmutable(child, varIter, term);
        }
        else if (child.rule() == "unaryOp")
        {
            // manage unaryOps
        }
    }
}

void Statement::dfsImmutable(pnode node, std::pair<string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        // not yet implemented
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
            dfsConstant(child, varIter, term);
        }
        else
        {
            dfsExpr(child, varIter);
        }
    }
}

void Statement::dfsConstant(pnode node, std::pair<string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        if (child.rule () == "NUMCONST")
        {
            if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
            }
        }
        else if (child.rule() == "CHARCONST")
        {
            if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
            }
        }
        else if (child.rule() == "true")
        {
            if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
            }
        }
        else if (child.rule() == "false")
        {
            if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
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
            irInstruction newTerm;
            dfsVarDeclInit(child, varIter, newTerm);
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

void Statement::dfsVarDeclInit(pnode node, std::pair<string, int> &varIter, irInstruction &term)
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
            dfsSimpleExpr(child, varIter, term);
        }
    }
}




void Statement::translateTerm(pnode node, std::pair<std::string, int> &varIter)
{
    string var = varIter.first;
    int i = varIter.second++;


}


void Statement::getLeftMostLeaf(pnode node, std::string &rule)
{
    if (node.children().empty())
    {
        rule = node.rule();
        return;
    }
    getLeftMostLeaf(node.children()[0], rule);
}

void Statement::getLeftMostLeafNode(pnode node, pnode &leafNode)
{
    if (node.children().empty())
    {
        leafNode = node;
        return;
    }
    getLeftMostLeafNode(node.children()[0], leafNode);
}
void Statement::moveRight(pnode node, pnode &leafNode)
{
    if (node.children().empty())
    {
        leafNode = node;
        return;
    }
    getLeftMostLeafNode(node.children()[node.children().size() - 1], leafNode);
}