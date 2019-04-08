//
// Created by mmalett on 4/7/19.
//

#include "instructions.h"

#include <algorithm>

Function::Function(pnode funcRoot)
{
    pnode compStmtRoot = funcRoot.children()[5].children()[0];
    // sends the root statement list of the compound statement.
    extractStatements(compStmtRoot.children()[1]);
}

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
            if (statement.children()[0].rule() == "exprStmt")
            {
                m_statementList.push_back(ExpressionStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "selStmt")
            {
                m_statementList.push_back(SelectionStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "iterStmt")
            {
                m_statementList.push_back(IterationStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "retStmt")
            {
                m_statementList.push_back(ReturnStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "breakStmt")
            {
                m_statementList.push_back(BreakStatement(statement.children()[0]));
            }
            else if (statement.children()[0].rule() == "varDecl")
            {
                m_statementList.push_back(VariableDeclaration(statement.children()[0]));
            }
        }
    }
}

void Statement::parseBreakStmt(pnode root, Statement &breakStmt)
{

}

void Statement::parseSelStmt(pnode root, Statement &selStmt)
{

}
void Statement::parseExprStmt(pnode root, Statement &exprStmt)
{
    if (root.children()[0].rule() == ";")
    {
        return;
    }
    if (root.children()[0].rule() == "expr")
    {
        // DFS and return the leaves in the proper order.
        for (auto child : root.children()[0].children())
        {
            std::pair<std::string, int> varIter;
            dfsExpr(child, varIter);
        }
    }
}

void Statement::parseIterStmt(pnode root, Statement &iterStmt)
{

}

void Statement::parseRetStmt(pnode root, Statement &retStmt)
{

}
void Statement::parseVarDecl(pnode root, Statement &varDecl)
{

}

void Statement::dfsExpr(pnode node, std::pair<std::string, int> &varIter)
{
    // the left hand side of an expression.
    if (node.rule() == "mutable" && node.parent()->rule() == "expr")
    {
        // get var name and place in here m_varIter map.
        varIter.first = node.children()[0].children()[0].rule();
        varIter.second = 0;
    }
    else if (node.rule() == "mutUnaryOp")
    {

    }
    else if (node.rule() == "mutBinOp" && node.parent()->rule() == "expr")
    {
        // only handling assignment to a single variable for now

    }
    else // simpleExpr
    {
        for (auto child : node.children())
        {
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
        }
    }
}
void Statement::translateTerm(pnode node, std::pair<std::string, int> &varIter)
{
    string var = varIter.first;
    int i = varIter.second++;


}