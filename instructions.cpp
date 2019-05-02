//
// Created by mmalett on 4/7/19.
// Co-written by Victor ¯\_(ツ)_/¯

#include "instructions.h"

#include <algorithm>
#include <iostream>

using namespace std;

Function::Function(pnode funcRoot, SymbolTable symbolTable)
{
    // ugly but it works ¯\_(ツ)_/¯
    m_funcHeader.op = "FUNC";
    m_funcHeader.res = funcRoot.children()[1].children()[0].rule();
    this->m_symbolTable = symbolTable.children().at(m_funcHeader.res);
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


void Statement::parseBreakStmt(pnode &root, Statement &breakStmt)
{
    // this could be tricky?
    irInstruction brk;
    brk.op = "BREAK";
    brk.res = m_endLabel;
    m_curTerms.push_back(brk);
}

void Statement::parseSelStmt(pnode &root, Statement &selStmt)
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
        auto statement = SelectionStatement(node.children()[0], m_symbolTable);
        m_statements.push_back(statement);

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
        m_statements.push_back(BreakStatement(node.children()[0], m_symbolTable, this->m_endLabel));
    }
    else if (node.children()[0].rule() == "varDecl")
    {
        m_statements.push_back(VariableDeclaration(node.children()[0], m_symbolTable));
    }
    else if (node.children()[0].rule() == "compStmt")
    {
        m_statements.push_back(CompoundStatement(node.children()[0], m_symbolTable, OTHER));
    }
}

void Statement::dfsIfStmt(pnode &node, std::pair<string, int> &varIter)
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
            //dfsStmt(child);
            dfsIfStmt(child, varIter);
        }
        else if (child.rule() == "compStmt")
        {
            m_statements.push_back(CompoundStatement(child, m_symbolTable, IFCMP));
        }
    }
}

void Statement::dfsElseStmt(pnode &node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "else")
        {
            // discard symbol and set the appropriate behavior.
        }
        else if (child.rule() == "stmt")
        {
            dfsElseStmt(child, varIter);
        }
        else if (child.rule() == "compStmt")
        {
            m_statements.push_back(CompoundStatement(child, m_symbolTable, ELSECMP));
            //parseCmpStmt(child, *this, ELSECMP);
        }
        else
        {
            dfsElseStmt(child, varIter);
        }
    }
}

void Statement::dfsCompStmt(pnode &node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "{")
        {
            // discard symbol
        }
        else if (child.rule() == "}")
        {

        }
        else if(child.rule() == "stmt")
        {
            dfsStmt(child);
        }
        else if (child.rule() == "stmtList")
        {
            dfsCompStmt(child, varIter);
        }
    }
}

void Statement::parseExprStmt(pnode &root, Statement &exprStmt)
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

void Statement::parseIterStmt(pnode &root, Statement &iterStmt)
{
    irInstruction inst;
    inst.block = m_startLabel;
    m_curTerms.push_back(inst);

    pair<string, int> varIter = make_pair("",-1);
    dfsIterStmt(root, varIter);
}

void Statement::dfsIterStmt(pnode &node, std::pair<std::string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "while")
        {
            // note that we need to make a loop with what follows.
        }
        else if (child.rule() == "simpleExpr")
        {
            irInstruction newTerm;
            dfsSimpleExpr(child, varIter, newTerm);
        }
        else if (child.rule() == "(" || child.rule() == ")")
        {
            // ignore symbols
        }
        else if (child.rule() == "stmt")
        {
            //dfsStmt(child);
            dfsIterStmt(child, varIter);
        }
        else if (child.rule() == "compStmt")
        {
            Statement cmp = CompoundStatement(child, m_symbolTable, WHILECMP);
            cmp.m_startLabel = this->m_startLabel;
            m_statements.push_back(cmp);
        }
    }
}

void Statement::parseRetStmt(pnode &root, Statement &retStmt)
{
    irInstruction ret;
    ret.op = "RET";
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
            varIter.first = "_ret";
            dfsExpr(child, varIter);
            ret.op   = "COPY";

            if (!m_curTerms.empty())
            {
                ret.arg1 = m_curTerms.back().res;
                ret.res  = varIter.first;
                if (m_curTerms.back().res != ret.res)
                {
                    m_curTerms.push_back(ret);
                }
                irInstruction val;
                val.op = "RET";
                val.arg1 = varIter.first;
                m_curTerms.push_back(val);
            }
        }
    }
    m_curTerms.push_back(ret);
}
void Statement::parseVarDecl(pnode &root, Statement &varDecl)
{
    string lastVar;
    for (auto child : root.children())
    {
        if (child.rule() == "typeSpec")
        {
            // currently only goes to return type spec and that goes to terminals
            // so assign whatever terminals
        }
        else if (child.rule() == ";")
        {
            if (!m_curTerms.empty() && !lastVar.empty())
            {
                m_curTerms.back().res = lastVar;
            }
        }
        else if (child.rule() == "varDeclList")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            dfsVarDeclList(child, varIter);
            lastVar = varIter.first;
        }
        else if (child.rule() == "varDeclInit")
        {
            std::pair<std::string, int> varIter;
            varIter.second = -1;
            irInstruction newTerm;
            dfsVarDeclInit(child, varIter, newTerm);
            lastVar = varIter.first;
        }
        else if (child.rule() == "ID")
        {
            irInstruction expr;
            expr.op = "COPY";
            expr.arg1 = "0";
            expr.res = lastVar = child.children()[0].rule();
        }
    }
}

void Statement::parseCmpStmt(pnode &root, Statement &cmpStmt, stmt_type selStmt)
{
    irInstruction jmp;
    switch (selStmt)
    {
        case IFCMP:
            jmp.op = "CJMP";
            jmp.res = this->m_endLabel;
            m_curTerms.push_back(jmp);
        break;
        case ELSECMP:
            //m_curTerms.push_back(jmp);
        break;
        case ELIFCMP:
        break;
        case WHILECMP:
            jmp.op = "CJMP";
            jmp.res = this->m_endLabel;
            m_curTerms.push_back(jmp);
        break;
        case OTHER:
            break;
    }
    pair<string, int> varIter = make_pair("",-1);
    dfsCompStmt(root, varIter);
}

void Statement::dfsExpr(pnode &node, std::pair<std::string, int> &varIter)
{

    node.visited() = true;
    if (node.childCount() == 3 && !node.children()[node.childCount() - 1].visited())
    {
        if (varIter.first.empty())
        {
            getLeftMostLeaf(node.children()[0], varIter.first);
        }
        dfsExpr(node.children()[node.childCount() - 1], varIter);
    }
    irInstruction expr;
    for (auto child : node.children())
    {
        if (child.visited())
        {
            continue;
        }
        // the left hand side of an expression.
        if (child.rule() == "mutable")
        {
            if (varIter.first.empty())
            {
                getLeftMostLeaf(child, varIter.first);
            }
            if (expr.arg1.empty())
            {
                getLeftMostLeaf(child, expr.arg1);
            }
            else
            {
                getLeftMostLeaf(child, expr.arg2);
            }
        }
            // not supported
        else if (child.rule() == "mutUnaryOp")
        {
            // the op is prefix
            if (node.children()[0].rule() == child.rule())
            {
                // Insert an instruction to increment by 1 now, then continue.
                irInstruction expr;
                expr.op = child.children()[0].ins();
                getLeftMostLeaf(node.children()[1], expr.res);
                getLeftMostLeaf(node.children()[1], expr.arg1);
                expr.arg2 = "1";
                m_curTerms.push_back(expr);
            }
            // the op is postfix
            else if (node.children()[1].rule() == child.rule())
            {

            }
        }
        else if (child.rule() == "mutBinOp")
        {

        }
        else // simpleExpr
        {
            dfsSimpleExpr(child, varIter, expr);

            irInstruction var;
            var.op   = "COPY";

            if (m_curTerms.empty())
            {
                var.arg1 = expr.arg1;
                var.res  = varIter.first;
                m_curTerms.push_back(var);
            }
            else
            {
                var.arg1 = m_curTerms.back().res;
                var.res = varIter.first;
                if (m_curTerms.back().res != var.res && m_curTerms.back().op != "CALL")
                {
                    m_curTerms.push_back(var);
                }
            }
        }
    }
}
    // get result of the last term placed in


void Statement::processMutUnaryOp(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        //auto symbol = SymbolTable::symbolTable->lookup(child.rule());
        if (child.rule() == "++")
        {
            // postfix
            if (node.parent()->children()[0].rule() == "mutable")
            {
                term.op = "INC";
                m_curTerms.push_back(term);
            }
            // prefix
            else
            {
                term.op = "INC";
                m_curTerms.push_back(term);
            }
        }
        else if (child.rule() == "--")
        {
            // postfix
            if (node.parent()->children()[0].rule() == "mutable")
            {
                term.op = "DEC";
                m_curTerms.push_back(term);
            }
            // prefix
            else
            {
                term.op = "DEC";
                m_curTerms.push_back(term);
            }
        }
    }
}

void Statement::dfsSimpleExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{

    node.visited() = true;
    if (node.childCount() > 1
        && !node.children()[node.childCount() - 1].visited())
    {
        dfsUnaryRelExpr(node.children()[node.childCount() - 1], varIter, term);
    }
    for (auto child : node.children())
    {
        if (term.isNew())
        {
            if (varIter.first == "")
            {
                term.res = "_term" + to_string(++(varIter.second));
            }
            else
            {
                term.res = varIter.first + to_string(++(varIter.second));
            }
        }

        if (child.visited())
        {
            if (term.needsArg2() && !m_curTerms.empty())
            {
                term.arg2 = m_curTerms.back().res;
                term.res  = varIter.first + to_string(++(varIter.second));
                m_curTerms.push_back(term);
            }
            else if (term.needsArgs() && term.combineTerms(m_curTerms, term))
            {
                m_curTerms.push_back(term);
            }
            term.clear();

        }

        else if (child.rule() == "binLogOp")
        {
            // manage or op
            term.op = child.children()[0].ins();
        }
        else if (child.rule() == "unaryRelExpr")
        {
            dfsUnaryRelExpr(child, varIter, term);
        }
        else // simpleExpr
        {
            irInstruction newTerm;
            dfsSimpleExpr(child, varIter, newTerm);
        }
    }

}
void Statement::dfsUnaryRelExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    node.visited() = true;
    for (auto child : node.children())
    {
        if (child.rule() == "!")
        {
            // manage not op
            term.op = child.ins();
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

void Statement::dfsRelExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    node.visited() = true;
    if (node.childCount() > 1
    && !node.children()[node.childCount() - 1].visited())
    {
        dfsSumExpr(node.children()[node.childCount() - 1], varIter, term);
    }
    for (auto child : node.children())
    {
        if (child.visited())
        {

        }
        else if (child.rule() == "relOp")
        {
            // manage relOps
            term.op = child.children()[0].ins();
        }
        else
        {
            dfsSumExpr(child, varIter, term);
        }
    }
    if (term.complete())
    {
        m_curTerms.push_back(term);
        term.clear();
    }
}

void Statement::dfsSumExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{

    node.visited() = true;
    if (node.childCount() > 1
        && !node.children()[node.childCount() - 1].visited()
        && node.children()[node.childCount() - 1].childCount() > 1)
    {
        dfsTerm(node.children()[node.childCount() - 1], varIter, term);
    }

    for (auto child : node.children())
    {
        if (term.isNew())
        {
            if (varIter.first == "")
            {
                term.res = "_term" + to_string(++(varIter.second));
            }
            else
            {
                term.res = varIter.first + to_string(++(varIter.second));
            }
        }
        if (child.visited() && term.isMath())
        {
            if (term.needsArg2() && !m_curTerms.empty())
            {
                term.arg2 = m_curTerms.back().res;
                if (varIter.first == "")
                {
                    term.res = "_term" + to_string(++(varIter.second));
                }
                else
                {
                    term.res = varIter.first + to_string(++(varIter.second));
                }
                m_curTerms.push_back(term);
            }
            else if (term.needsArgs() && term.combineTerms(m_curTerms, term))
            {
                m_curTerms.push_back(term);
            }
            term.clear();
        }
        // handle cases
        else if (child.rule() == "sumOp")
        {
            // manage sumOps
            term.op = child.children()[0].ins();
        }
        else if (child.rule() == "term")
        {
            dfsTerm(child, varIter, term);
        }
        else
        {
            dfsSumExpr(child, varIter, term);
        }

    }
    if (term.complete())
    {
        m_curTerms.push_back(term);
        term.clear();
    }
}

void Statement::dfsTerm(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    node.visited() = true;
    for (auto child : node.children())
    {
        if (term.isNew())
        {
            if (varIter.first == "")
            {
                term.res = "_term" + to_string(++(varIter.second));
            }
            else
            {
                term.res = varIter.first + to_string(++(varIter.second));
            }        }
        if (child.visited())
        {
            if (term.needsArg2() && !m_curTerms.empty())
            {
                term.arg2 = m_curTerms.back().res;
                term.res  = varIter.first + to_string(++(varIter.second));
                m_curTerms.push_back(term);
                term.clear();
            }
        }
        // handle cases
        else if (child.rule() == "mulOp")
        {
            // manage mulOps
            term.op = child.children()[0].ins();
        }
        else if (child.rule() == "term")
        {
            if (child.childCount() > 1)
            {
                dfsTerm(child, varIter, term);
                //m_curTerms.push_back(term);
            }
            else
            {
                dfsUnaryExpr(child.children()[0], varIter, term);
            }
        }
        else
        {
            dfsUnaryExpr(child, varIter, term);
            if (term.needsArg2() && !m_curTerms.empty())
            {
                term.arg2 = m_curTerms.back().res;
                if (varIter.first == "")
                {
                    term.res = "_term" + to_string(++(varIter.second));
                }
                else
                {
                    term.res = varIter.first + to_string(++(varIter.second));
                }                m_curTerms.push_back(term);
                term.clear();
            }
            if (term.complete())
            {
                m_curTerms.push_back(term);
                term.clear();
            }
        }
    }
}

void Statement::dfsUnaryExpr(pnode &node, std::pair<string, int> &varIter, irInstruction &term)
{
    string unaryOp;
    node.visited() = true;
    for (auto child : node.children())
    {
        if (child.rule() == "mutable")
        {
           string check = child.children()[0].children()[0].rule();
           if (m_symbolTable.lookup(check) == NULL) {
               cerr << "warning symbol: " << check << " not found in scope " << m_symbolTable.scope() << endl;
               //exit(1);
           }

           //term.res = term.arg1 + to_string(++(varIter.second));
           /* if (term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
                term.arg1 = unaryOp + term.arg1;
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
                term.arg2 = unaryOp + term.arg2;
            }
            unaryOp = "";
            */
        }
        else if (child.rule() == "immutable")
        {
            dfsImmutable(child, varIter, term);
            /*
            if (!term.arg1.empty())
            {
                getLeftMostLeaf(child, term.arg1);
                term.arg1 = unaryOp + term.arg1;
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
                term.arg2 = unaryOp + term.arg2;
            }
            unaryOp = "";
            /*

        }
        else if (child.rule() == "unaryOp")
        {
            // manage unaryOps
            getLeftMostLeaf(child, unaryOp);

        }
    }
}

void Statement::dfsImmutable(pnode &node, std::pair<string, int> &varIter, irInstruction &term)
{
    node.visited() = true;
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

void Statement::dfsConstant(pnode &node, std::pair<string, int> &varIter, irInstruction &term)
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
                term.arg1 = "\'" + term.arg1 + "\'";
            }
            else
            {
                getLeftMostLeaf(child, term.arg2);
                term.arg2 = "\'" + term.arg2 + "\'";
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

void Statement::dfsCall(pnode &node, std::pair<string, int> &varIter)
{
    irInstruction call;
    call.op = "CALL";
    for (auto child : node.children())
    {
        if (child.rule() == "ID")
        {
            // manage variable name
            call.res = child.children()[0].rule();
            m_curTerms.push_back(call);
            //m_curTerms.back().op = child.children()[0].ins();

        }
        else if (child.rule() == "args")
        {
            dfsArgs(child, varIter);
        }
    }
}

void Statement::dfsArgs(pnode &node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == "EMPTY")
        {
            // done here
            //m_curTerms.back().params = nullptr;
        }
        else
        {
            //irInstruction pushOp;
            //m_curTerms.push_back(pushOp);
            dfsArgList(child, varIter);
        }
    }
}

void Statement::dfsArgList(pnode &node, std::pair<string, int> &varIter)
{
    for (auto child : node.children())
    {
        if (child.rule() == ",")
        {
            // throw away symbol
            //irInstruction pushOp;
            //m_curTerms.push_back(pushOp);
        }
        else if (child.rule() == "expr")
        {

            //m_curTerms.back().op = "push";
            dfsExpr(node, varIter);
        }
        else
        {
            dfsArgList(child, varIter);
        }
    }
}

void Statement::dfsVarDeclList(pnode &node, std::pair<string, int> &varIter)
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
            if (!m_curTerms.empty() && !varIter.first.empty())
            {
                m_curTerms.back().res = varIter.first;
            }
        }
        else if (child.rule() == "ID")
        {

            varIter.first = child.children()[0].ins();
        }
        else if (child.rule() == "=")
        {
            // discard symbol
        }
        else if (child.rule() == "simpleExpr")
        {
            irInstruction expr;
            dfsSimpleExpr(child, varIter, expr);
            expr.op = "COPY";
            m_curTerms.push_back(expr);
        }
        else if (child.rule() == "varDeclList")
        {
            pair<string, int> varIter;
            dfsVarDeclList(child, varIter);
        }
    }
    if (!m_curTerms.empty() && !varIter.first.empty())
    {
        m_curTerms.back().res = varIter.first;
    }
}

void Statement::dfsVarDeclInit(pnode &node, std::pair<string, int> &varIter, irInstruction &term)
{
    for (auto child : node.children())
    {
        if (child.rule() == "ID")
        {
            // begin the variable name
            varIter.first = child.children()[0].ins();

        }
        else if (child.rule() == "simpleExpr")
        {
            dfsSimpleExpr(child, varIter, term);
            irInstruction var;

            term.op = var.op = "COPY";
            if (term.complete())
            {
                m_curTerms.push_back(term);
            }
            else if (m_curTerms.empty())
            {
                var.arg1 = term.arg1;
                var.res  = varIter.first;
                m_curTerms.push_back(var);

            }
            else
            {
                var.arg1 = m_curTerms.back().res;
                var.res = varIter.first;
                if (m_curTerms.back().res != var.res)
                {
                    m_curTerms.push_back(var);
                }
            }

        }
    }
    // we have found an uninitialized declaration.
    if (!varIter.first.empty() && varIter.first != m_curTerms.back().res && term.isNew())
    {
        term.op = "COPY";
        term.res = varIter.first;
        term.arg1 = "0"; // uninitialized
        m_curTerms.push_back(term);
        varIter.first.clear();
    }
}




void Statement::translateTerm(pnode &node, std::pair<std::string, int> &varIter)
{
    string var = varIter.first;
    int i = varIter.second++;


}


void Statement::getLeftMostLeaf(pnode &node, std::string &rule)
{
    if (node.children().empty())
    {
        rule = node.rule();
        return;
    }
    getLeftMostLeaf(node.children()[0], rule);
}

void Statement::getLeftMostLeafNode(pnode &node, pnode &leafNode)
{
    if (node.children().empty())
    {
        leafNode = node;
        return;
    }
    getLeftMostLeafNode(node.children()[0], leafNode);
}
void Statement::moveRight(pnode &node, pnode &leafNode)
{
    if (node.children().empty())
    {
        leafNode = node;
        return;
    }
    getLeftMostLeafNode(node.children()[node.children().size() - 1], leafNode);
}


void Statement::processMutBinaryOp(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term)
{
    string op(node.children()[0].rule());
    switch (op[0])
    {
        case '=': // assignment
        {
            // nothing to do for this
        }
        case '+': // +=
        {
            // needs implementation
        }
        case '-': // -=
        {
            // needs implementation
        }
        case '*': // *=
        {
            // needs implementation
        }
        case '/': // /=
        {
            // needs implementation
        }
        default:
        {
            // shouldnt ever happen since the parser will complain.
        }
    }
}

void Statement::setInstructions(vector<irInstruction> &instructions, int &numBlocks, string funcName)
{
    for(auto term : m_curTerms)
    {
        if (term.op == "BREAK")
        {
            term.op = "JMP";
            term.res = this->m_endLabel;
            //continue;
        }
        instructions.push_back(term);
    }
    for(int i = 0; i < m_statements.size(); i++)
    {
        m_statements[i].setInstructions(instructions, numBlocks, funcName);
        if (is_If(m_statements[i]) && m_statements[i+1].m_type == ELSECMP)
        {
            irInstruction jmp;
            jmp.op = "JMP";
            jmp.res = m_statements[i+1].m_endLabel;
            instructions.push_back(jmp);
        }
        if (m_statements[i].m_type == WHILECMP)
        {
            irInstruction jmp;
            jmp.op = "JMP";
            jmp.res = m_statements[i].m_startLabel;
            instructions.push_back(jmp);
            irInstruction b_end;
            b_end.block = m_statements[i].m_endLabel;
            instructions.push_back(b_end);
        }
        if (is_If(m_statements[i]) || m_statements[i].m_type == ELSECMP)
        {
            irInstruction b_end;
            b_end.block = m_statements[i].m_endLabel;
            instructions.push_back(b_end);
        }
    }
}

bool Statement::is_If(Statement stmt)
{
    if (stmt.m_type == IFCMP || stmt.m_type == ELIFCMP) {
        return true;
    }
    return false;
}
