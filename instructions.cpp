//
// Created by mmalett on 4/7/19.
//

#include "instructions.h"

#include <algorithm>

Function::Function(pnode funcRoot)
{
    pnode compStmtRoot = funcRoot.children()[5].children()[0];
    // sends the root statement list of the compound statement.
    m_statementList = extractStatements(compStmtRoot.children()[1]);
}
// takes the root node of a statement list
std::vector<Statement> Function::extractStatements(pnode root)
{
    std::vector<Statement> statementList;
    for (auto statement : root.children())
    {
        if (statement.rule() == "stmtList")
        {
            extractStatements(statement);
        }
        else if (statement.rule() == "stmt")
        {
            if (statement.children()[0].rule() == "exprStmt")
            {
                statementList.push_back(ExpressionStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "selStmt")
            {
                statementList.push_back(SelectionStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "iterStmt")
            {
                statementList.push_back(IterationStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "retStmt")
            {
                statementList.push_back(ReturnStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "breakStmt")
            {
                statementList.push_back(BreakStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "varDecl")
            {
                statementList.push_back(VariableDeclaration(statement.children()[0]));
            }
        }
    }
}

void Statement::parseBreakStmt(pnode root, Statement &breakStmt)
{
    // this could be tricky.
}

void Statement::parseSelStmt(pnode root, Statement &selStmt)
{
    for (auto child : root.children())
    {

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
            Function::extractStatements(child);
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
            // get var name and place in here m_varIter map.
            varIter.first  = child.children()[0].children()[0].rule();
            varIter.second = 0;
        }
        else if (child.rule() == "mutUnaryOp")
        {
            // manage op
        }
        else if (child.rule() == "mutBinOp")
        {
            // only handling assignment to a single variable for now
        }
        else // simpleExpr
        {
            dfsSimpleExpr(node, varIter);
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

        /*
        // respect precedence, reorder terms
        std::vector<pnode> terms = child.children();
        if (child.rule() == "sumExpr")
        {
            // a descending order stable sort (term should come before sumExpr in precedence order)
            std::stable_sort(terms.begin(), terms.end(), rCompNode);
        }
        else if (child.rule() == "term" && child.parent()->rule() == "term")
        {
            translateTerm(child, varIter);
        }
        // leaf node
        if (terms.empty())
        {
            if (child.rule() == ";" || child.rule() == "(" || child.rule() == ")")
            {
                return;
            }
            else
            {
                // rule = child.rule();
                return;
            }
        }
        else
        {
            dfsExpr(child, varIter);
        }
        */
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
            // manage mutable
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
        }
        else
        {
            dfsExpr(child, varIter);
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
        }
        else
        {
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
        }
        else if (child.rule() == "expr")
        {
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
        }
        else if (child.rule() == "=")
        {
            // set up assignment
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