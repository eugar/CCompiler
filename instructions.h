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

// determines what type of compound statement
// used for parsetree -> IR generation
typedef enum {
    IFCMP, ELSECMP, ELIFCMP, ITERCMP, OTHER
} stmt_type;

typedef struct irInstruction
{
    // default constructor that zeroes out all values.
    irInstruction()
    : op()
    , arg1()
    , params()
    , arg2()
    , res()
    , block()
    {}

    string op;
    string arg1;
    pnode *params;
    string arg2;
    string res;
    string block;

    void clear()
    {
        op.clear();
        arg1.clear();
        params = nullptr;
        arg2.clear();
        res.clear();
        block.clear();
    }
    // all cases of possible ops are NOT accounted for.
    bool complete()
    {
        // require op, arg1, arg2 and res
        if (op == "SUB" || op == "ADD" || op == "MUL"
        || op == "DIV" || op == "GRTH" || op == "LSTH"
        || op == "GREQ" || op == "LSEQ" || op == "EQ")
        {
            return !(op.empty() || arg1.empty() || arg2.empty() || res.empty());
        }
        // require op, arg1 and res
        else if (op == "COPY" || op == "NOT")
        {
            return !(op.empty() || arg1.empty() || res.empty());
        }
        // require op and arg1
        else if (op == "RET")
        {
            return !(op.empty() || arg1.empty());
        }
        else
        {
            return false;
        }
    }
    bool isMath()
    {
        return op == "ADD" || op == "SUB" || op == "MUL" || op == "DIV";
    }
    bool needsArgs()
    {
        return !op.empty() && arg1.empty() && arg2.empty() && !res.empty();
    }
    bool needsArg2()
    {
        return !op.empty() && !arg1.empty() && arg2.empty() && !res.empty();
    }
    bool isNew()
    {
        return op.empty() && arg1.empty() && arg2.empty() && res.empty();
    }
    bool combineTerms(std::vector<irInstruction> &curTerms, irInstruction &term)
    {
        // find term that wasn't carried through
        std::string prevArg;
        for (auto ins = curTerms.end() - 1; ins >= curTerms.begin(); ins--)
        {
            if (term.arg1.empty())
            {
                term.arg1 = ins->res;
                prevArg = ins->res;
            }
            if (prevArg == ins->res)
            {
                if (prevArg[0] == ins->arg2[0])
                {
                    prevArg = ins->arg2;
                }
                else if (prevArg[0] == ins->arg1[0])
                {
                    prevArg = ins->arg1;
                }
                else
                {
                    term.arg2 = (--ins)->res;
                }
            }

            if (term.complete())
            {
                return true;
            }
        }
        return false;
    }

} irInstruction;

class Statement;
class VariableDeclaration;
class Function
{
public:

    Function(pnode funcRoot, SymbolTable symbolTable);
    void extractStatements(pnode root);
    std::vector<Statement> getStatementList(){return m_statementList;}
    irInstruction funcHeader(){return m_funcHeader;}
private:
    irInstruction m_funcHeader;
    std::vector<Statement> m_statementList;
    SymbolTable m_symbolTable;
};

// Abstract base class
class Statement
{
public:

    Statement(pnode &node, SymbolTable symbolTable)
            : m_root(node),
            m_symbolTable(symbolTable)
    {
        dfsStmt(m_root);
    }

    static bool rCompNode(pnode a, pnode b) {return a.rule() >= b.rule();}

    void setInstructions(vector<irInstruction> &instructions, int &numBlocks, string funcName);

    std::vector<irInstruction> getCurTerms(){return m_curTerms;}

    virtual void parseExprStmt(pnode &root, Statement &exprStmt);
    virtual void parseSelStmt(pnode &root, Statement &selStmt);
    virtual void parseIterStmt(pnode &root, Statement &iterStmt);
    virtual void parseRetStmt(pnode &root, Statement &retStmt);
    virtual void parseBreakStmt(pnode &root, Statement &breakStmt);
    virtual void parseVarDecl(pnode &root, Statement &varDecl);
    virtual void parseCmpStmt(pnode &root, Statement &compStmt, stmt_type selStmt);

    pnode m_root;
    std::vector<Statement> m_statements;

private:
    void dfsStmt(pnode node);
    void dfsCompStmt(pnode &node, std::pair<std::string, int> &varIter);
    void dfsExpr(pnode &node, std::pair<std::string, int> &varIter);
    void dfsSimpleExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsAndExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsUnaryRelExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsRelExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsSumExpr(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsTerm(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void dfsUnaryExpr(pnode &node, std::pair<string, int> &varIter, irInstruction &term);
    void dfsImmutable(pnode &node, std::pair<string, int> &varIter, irInstruction &term);
    void dfsConstant(pnode &node, std::pair<string, int> &varIter, irInstruction &term);
    void dfsCall(pnode &node, std::pair<string, int> &varIter);
    void dfsIfStmt(pnode &node, std::pair<string, int> &varIter);
    void dfsElifStmt(pnode &node, std::pair<string, int> &varIter);
    void dfsElseStmt(pnode &node, std::pair<string, int> &varIter);
    void dfsArgs(pnode &node, std::pair<string, int> &varIter);
    void dfsArgList(pnode &node, std::pair<string, int> &varIter);
    void dfsVarDeclList(pnode &node, std::pair<string, int> &varIter);
    void dfsVarDeclInit(pnode &node, std::pair<string, int> &varIter, irInstruction &term);

    void translateTerm(pnode &node, std::pair<std::string, int> &varIter);

    void processMutUnaryOp(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);
    void processMutBinaryOp(pnode &node, std::pair<std::string, int> &varIter, irInstruction &term);

    void getLeftMostLeaf(pnode &node, std::string &rule);
    void getLeftMostLeafNode(pnode &node, pnode &leafNode);
    void moveRight(pnode &node, pnode &leafNode);
    bool inline is_If(Statement stmt);

    SymbolTable m_symbolTable;

protected:
    std::vector<irInstruction> m_curTerms;
    stmt_type m_type;
    string m_endLabel;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseExprStmt(node, *this);
        m_type = OTHER;
    }
    ~ExpressionStatement() = default;

};

class SelectionStatement : public Statement
{
public:
    SelectionStatement(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseSelStmt(node, *this);
        m_type = OTHER;
    }
    ~SelectionStatement() = default;
    void pushEndSel(irInstruction inst){this->m_curTerms.push_back(inst);}
};

class IterationStatement : public Statement
{
public:
    IterationStatement(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseIterStmt(node, *this);
        m_type = ITERCMP;
    }
    ~IterationStatement() = default;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseRetStmt(node, *this);
        m_type = OTHER;
    }
    ~ReturnStatement() = default;
};

class BreakStatement : public Statement
{
public:
    BreakStatement(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseBreakStmt(node, *this);
        m_type = OTHER;
    }
    ~BreakStatement() = default;
};


class VariableDeclaration : public Statement
{
public:
    VariableDeclaration(pnode &node, SymbolTable symbolTable)
    : Statement(node, symbolTable)
    {
        parseVarDecl(node, *this);
        m_type = OTHER;
    }
    ~VariableDeclaration() = default;
};

class CompoundStatement : public Statement
{
public:
    CompoundStatement(pnode &node, SymbolTable symbolTable, stmt_type selStmt)
    : Statement(node, symbolTable)
    {
        m_type = selStmt;
        m_endLabel = "_blockEnd" + to_string(rand()%100);
        parseCmpStmt(node, *this, selStmt);
    }
    ~CompoundStatement() = default;
};

#endif //CCOMPILER_INSTRUCTIONS_H
