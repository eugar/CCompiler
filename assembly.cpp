//
// Created by eugene on 2019-04-11.
//

#include <iostream>
#include "assembly.h"

using namespace std;

Assembly::Assembly(string filename) {
    this->out.open(filename);

    if (!out.is_open())
    {
        cout << "File " << filename << " could not be opened to write the assembly code file." << endl;
        exit(1);
    }

    bbcount = 1;
    registerNum = 0;
}

void Assembly::writeFunctionPrologue() {
    writeInstruction("pushq\t%rbp");
    writeInstruction("movq\t%rsp, %rbp");
    writeInstruction("xorl\t%eax, %eax"); // zero out eax, the result register
}

void Assembly::writeFunctionEpilogue() {
    writeInstruction("popq\t %rbp");
    writeInstruction("ret");
}

void Assembly::writeHeader() {
    out << ".section .data" << endl;
    out << ".section .bss" << endl;
    out << ".section .text" << endl;
    out << ".globl _start" << endl;
    out << "#.bb0:" << endl;
    out << "_start:" << endl;
    writeFunctionPrologue();
    out << "" << endl;
    writeInstruction("call main"); // need to do a check to make sure that main exist
    writeInstruction("movq\t%rax, %rbx");
    writeInstruction("movq\t$60, %rax #sys_exit ");
    writeInstruction("movq\t$0x0, %rdi");
    writeInstruction("syscall");
}

void Assembly::generateCode(vector<irInstruction> instructions)
{
    writeHeader();
    for (auto instruction : instructions)
    {
        insertBB(instruction);
    }
}

void Assembly::insertBB(irInstruction ins)
{
    if (ins.op == "FUNC" || ins.op == "LABEL")
    {
        out << "#.bb" << this->bbcount << ":" << endl;
        bbcount++;
    }

    chooseInstruction(ins);
}

string Assembly::createString(string argument)
{
    if (argument.find_first_not_of("0123456789") != std::string::npos) // a char was found, it is an id
    {
        //lookup in the table
        int regNum = this->assemblyContext.getRegister(argument);
        if (regNum != -1) // it was found
        {
            return "%r" + to_string(regNum);
        }
        else
        {
            cout << "variable " + argument + " was not found while generating assembly";
            exit(1);
        }
    }

    return "$" + argument;
}

int Assembly::getNextReg() {
    int tmpReg = this->registerNum;
    this->registerNum++;

    return tmpReg;
}

//this function takes in strings and formats them. After
//the strings have been formatted they are written out to the
//output .s file
void Assembly::writeInstruction(string line)
{ //todo: format correctly (pretty print)
    out << "  " << line << endl;
}

//this function takes in an irInstruction and chooses the correct
//x86 instruction (AT&T syntax) and sends a string to writeInstruction()
//to be formatted
void Assembly::chooseInstruction(irInstruction ins)
{ //todo: choose appropiate instructions, e.g., addq, addw, etc
    if (ins.op == "NOT") // unary instructions
    {
        int tmpRegNum = getNextReg();
        writeInstruction(createString(ins.arg1));
        writeInstruction("not\t%eax");
        writeInstruction("mov\t%eax, %r" + to_string(tmpRegNum));
    }
    else if (ins.op == "ADD" || ins.op == "SUB" || ins.op == "MUL" || ins.op == "DIV")
    {
        int tmpRegNum = getNextReg();
        writeInstruction("mov\t" + createString(ins.arg2) + ", %eax");
        writeInstruction("mov\t" + createString(ins.arg1) + ", %ecx");

        if (ins.op == "ADD")
        {
            writeInstruction("add\t%ecx, %eax");
        }
        else if (ins.op == "SUB")
        {
            writeInstruction("sub\t%ecx, %eax");
        }
        else if (ins.op == "MUL")
        {
            writeInstruction("mul\t%ecx, %eax");
        }
        else if (ins.op == "DIV")
        {

        }
        writeInstruction("mov\t%eax, %r" + to_string(tmpRegNum));
        this->assemblyContext.fillRegister(ins.res, tmpRegNum);
    }
    else if (ins.op == "EQ" || ins.op == "NOTEQ" || ins.op == "LSTH" || ins.op == "GRTH" || ins.op == "GREQ" || ins.op == "LSEQ") //binary instructions
    {
        if (ins.op == "EQ")
        {

        }
        else if (ins.op == "NOTEQ")
        {

        }
        else if (ins.op == "LSTH")
        {

        }
        else if (ins.op == "GRTH")
        {

        }
        else if (ins.op == "GREQ")
        {

        }
        else if (ins.op == "LSEQ")
        {

        }

    }
    else if (ins.op == "COPY")
    {
    }
    else if (ins.op == "RET")
    {
        writeFunctionEpilogue();
    }
    else if (ins.op == "FUNC") // generate a label
    {
        //todo: need to insert logic to handle scoping
        out << ins.arg1 << ":" << endl;
        writeFunctionPrologue();
    }
    else if (ins.op == "LABEL")
    {
        out << ins.arg1 << ":" << endl;
    }
}