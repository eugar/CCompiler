//
// Created by eugene on 2019-04-11.
//

#include <iostream>
#include <algorithm>
#include "assembly.h"
#include <algorithm>
#define TARGET_OS_MAC 0
using namespace std;

Assembly::Assembly(vector<irInstruction> instructions, string filename) {
    this->out.open(filename);
    this->instructions = instructions;

    if (!out.is_open())
    {
        cout << "File " << filename << " could not be opened to write the assembly code file." << endl;
        exit(1);
    }

    bbcount = 0;
    insCount = 0;
    gotoString = "";
    andFlag = 0;
    orFlag = 0;
}

void Assembly::writeFunctionPrologue() {
    writeInstruction("pushq\t\t%rbp");
    writeInstruction("movq\t\t%rsp, %rbp");
    writeInstruction("xorl\t\t%eax, %eax"); // zero out eax, the result register
}

void Assembly::writeFunctionEpilogue() {
    writeInstruction("movq\t\t%rbp, %rsp");
    writeInstruction("popq\t\t%rbp");
    writeInstruction("ret");
}

void Assembly::writeHeader() {
    out << ".globl _main" << endl;
}

void Assembly::generateCode(vector<irInstruction> instructions)
{
    writeHeader();

    for (auto instruction : instructions)
    {
        if (instruction.op == "GRTH" || instruction.op == "LSTH" || instruction.op == "GREQ" || instruction.op == "LSEQ"
        || instruction.op == "EQ" || instruction.op == "NOTEQ")
        {
            getGotoString();
            countConds();
        }
        insertBB(instruction);
        this->insCount++;
    }
}

void Assembly::insertBB(irInstruction ins)
{
    if (ins.op == "FUNC" || ins.op == "LABEL" || !(ins.block.empty()))
    {
        out << "#.bb" << this->bbcount << ":" << endl;
        bbcount++;
    }

    chooseInstruction(ins);
}

int Assembly::getNextOffset(string argument, int type) {
    if (!(argument[0] == '\'') && argument.find_first_not_of("0123456789") != std::string::npos) // a char was found, it is an id
    {
        //lookup in the table
        int offset = this->assemblyContext.getOffset(argument);
        if (offset != -1) // it was found
        {
            return offset;
        }
    }

    this->assemblyContext.context.stackOffset -= type;


    return this->assemblyContext.context.stackOffset;
}

void Assembly::countConds() {
    int tmp = this->insCount + 1;

    while ((this->instructions.at(tmp).op != "FUNC") && (tmp < (this->instructions.size() - 1)) && (this->instructions.at(tmp).block.empty()) && (this->instructions.at(tmp).op != "JMP") && (this->instructions.at(tmp).op != "CJMP"))
    {
        if (this->instructions.at(tmp).op == "AND")
        {
            this->andFlag++;
        }
        else if (this->instructions.at(tmp).op == "OR")
        {
            this->orFlag++;
        }
        tmp++;
    }
}

int Assembly::countLocalVars() {
    int tmp = this->insCount + 1;
    int size = 0;

    while (this->instructions.at(tmp).op != "FUNC" && tmp < this->instructions.size() - 1)
    {

        string result = this->instructions.at(tmp).res;
        if (!result.empty())
        {
            auto it = find_if(localVars.begin(), localVars.end(), [&result](const irInstruction & ins)
            {
                return ins.res == result;
            });

            if (it == localVars.end())
            {
                localVars.push_back(this->instructions.at(tmp));
                if (this->instructions.at(tmp).op == "ADD" || this->instructions.at(tmp).op == "SUB" || this->instructions.at(tmp).op == "MUL" || this->instructions.at(tmp).op == "COPY" || this->instructions.at(tmp).op == "CALL" || this->instructions.at(tmp).op == "DIV")
                {
                    size += 4;
                }
                else if (this->instructions.at(tmp).op == "LSTH" || this->instructions.at(tmp).op == "GRTH" || this->instructions.at(tmp).op == "AND" || this->instructions.at(tmp).op == "OR")
                {
                    size += 1;
                }
            }
        }
    tmp++;
    }

    return size;
}

void Assembly::getGotoString()
{
    int tmp = this->insCount;

    while (this->instructions.at(tmp).op != "CJMP" && tmp < this->instructions.size() - 1)
    {
        tmp++;
    }

    this->gotoString = this->instructions.at(tmp).res;
}


string Assembly::createString(string argument)
{
    if (!(argument[0] == '\'') && argument.find_first_not_of("0123456789") != std::string::npos) // a char was found, it is an id
    {
        //lookup in the table
        int offset = this->assemblyContext.getOffset(argument);
        if (offset != -1) // it was found
        {
            return to_string(offset) + "(%rbp)";
        }
        else
        {
            cout << "variable " + argument + " was not found while generating assembly\n";
            exit(1);
        }
    }

    return "$" + argument;
}

//this function takes in strings and formats them. After
//the strings have been formatted they are written out to the
//output .s file
void Assembly::writeInstruction(string line)
{ //todo: format correctly (pretty print)
    out << "    " << line << endl;
}

//this function takes in an irInstruction and chooses the correct
//x86 instruction (AT&T syntax) and sends a string to writeInstruction()
//to be formatted
void Assembly::chooseInstruction(irInstruction ins) {
    if (!ins.block.empty())
    {
        out << ins.block << ":" << endl;
    }
    if (ins.op == "NOT") // unary instructions
    {
        int tmp = getNextOffset(ins.res, 4);
        writeInstruction("movl\t\t" + createString(ins.arg1) + ", %eax");
        writeInstruction("not \t\t%eax");
        writeInstruction("movl\t\t%eax, " + to_string(tmp) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, tmp);
    }
    else if (ins.op == "ADD" || ins.op == "SUB" || ins.op == "MUL" || ins.op == "DIV")
    {
        if (ins.op == "ADD")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %ecx");
            writeInstruction("addl\t\t%ecx, %eax");
        }
        else if (ins.op == "SUB")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %ecx");
            writeInstruction("subl\t\t%ecx, %eax");
        }
        else if (ins.op == "MUL")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %ecx");
            writeInstruction("imull\t\t%ecx, %eax");
        }
        else if (ins.op == "DIV")
        { //todo: fix division bug
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %ecx");
            writeInstruction("movl\t\t$0, %edx");
            writeInstruction("cdq");
            writeInstruction("divl\t\t%ecx");
        }
        int tmp = getNextOffset(ins.res, 4);
        writeInstruction("movl\t\t%eax, " + to_string(tmp) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, tmp);
    }
    else if (ins.op == "EQ" || ins.op == "NOTEQ" || ins.op == "LSTH" || ins.op == "GRTH" || ins.op == "GREQ" || ins.op == "LSEQ") //binary instructions
    {
        if (ins.op == "EQ")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("sete\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("jne \t\t" + this->gotoString);
            }
        }
        else if (ins.op == "NOTEQ")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("setne\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("je  \t\t" + this->gotoString);
            }

        }
        else if (ins.op == "LSTH")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("setl\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("jge \t\t" + this->gotoString);
            }
        }
        else if (ins.op == "GRTH")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("setg\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("jle \t\t" + this->gotoString);
            }
        }
        else if (ins.op == "GREQ")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("setge\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("jl  \t\t" + this->gotoString);
            }

        }
        else if (ins.op == "LSEQ")
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("movl\t\t" + createString(ins.arg1) + ", %edx");
            writeInstruction("cmpl\t\t%edx, %eax");
            if (this->andFlag || this->orFlag) // save for later
            {
                writeInstruction("setle\t\t%bl");
                int tmp = getNextOffset(ins.res, 1);
                writeInstruction("movb\t\t%bl, " + to_string(tmp) + "(%rbp)");
                this->assemblyContext.setOffset(ins.res, tmp);
            }
            else
            {
                writeInstruction("jg  \t\t" + this->gotoString);
            }

        }


    }
    else if (ins.op == "AND")
    {
        this->andFlag--;

        writeInstruction("movb\t\t" + createString(ins.arg1) + ", %bl");
        writeInstruction("movb\t\t" + createString(ins.arg2) + ", %bh");
        writeInstruction("and\t\t%bh, %bl");

        if (this->andFlag || this->orFlag)
        {
            int offset = getNextOffset(ins.res, 1); // save for later
            writeInstruction("movb\t\t%bl, " + to_string(offset) + "(%rbp)");
            this->assemblyContext.setOffset(ins.res, offset);
        }
        else
        {
            writeInstruction("je  \t\t" + gotoString);
        }

    }
    else if (ins.op == "OR")
    {
        this->orFlag--;
        writeInstruction("movb\t\t" + createString(ins.arg1) + ", %bl");
        writeInstruction("movb\t\t" + createString(ins.arg2) + ", %bh");
        writeInstruction("or  \t\t%bh, %bl");

        if (this->orFlag || this->andFlag)
        {
            int offset = getNextOffset(ins.res, 1); // save for later
            writeInstruction("movb\t\t%bl, " + to_string(offset) + "(%rbp)");
            this->assemblyContext.setOffset(ins.res, offset);
        }
        else
        {
            writeInstruction("je\t\t" + gotoString);
        }

    }
    else if (ins.op == "COPY")
    {
        int offset = getNextOffset(ins.res, 4);
        writeInstruction("movl\t\t" + createString(ins.arg1) + ", %eax");
        writeInstruction("movl\t\t%eax, " + to_string(offset) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, offset);
    }
    else if (ins.op == "RET")
    {
        writeInstruction("movl\t\t" + createString(ins.arg1) + ", %eax");// move the return value in to the return register
        writeFunctionEpilogue();
    }
    else if (ins.op == "FUNC") // generate a label
    {
        this->localVars.clear();
        if (ins.res == "main")
        {
            ins.res = "_main";
        }
        out << ins.res << ":" << endl;
        writeFunctionPrologue();
        writeInstruction("subq\t\t$" + to_string(countLocalVars()) + ", %rsp"); // make room for local vars in this stack frame
        this->assemblyContext.newScope();
    }
    else if (ins.op == "CALL")
    {
        int offset = getNextOffset(ins.res, 4);
        writeInstruction("call\t\t" + ins.res);
        writeInstruction("movl\t\t%eax, " + to_string(offset) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, offset);
    }
    else if (ins.op == "JMP")
    {
        writeInstruction("jmp \t\t" + ins.res);
    }
    else if (ins.op == "LABEL")
    {
        out << ins.arg1 << ":" << endl;
        writeFunctionPrologue();
        writeInstruction("subq\t\t$" + to_string(countLocalVars()) + ", %rsp"); // make room for local vars in this stack frame
        this->assemblyContext.newScope();
    }
    else if (!ins.block.empty())
    {
    }
}
