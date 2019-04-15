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
}

void Assembly::writeFunctionPrologue() {
    writeInstruction("pushq\t%rbp");
    writeInstruction("movq\t%rsp, %rbp");
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
    writeInstruction("movq\t$60, %rax #sys_exit");
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
        writeInstruction("not\t%eax");
    }
    else if (ins.op == "ADD" || ins.op == "SUB" || ins.op == "MUL" || ins.op == "DIV" || ins.op == "EQ" || ins.op == "NOTEQ" || ins.op == "LSTH" || ins.op == "GRTH" || ins.op == "GREQ" || ins.op == "LSEQ") //binary instructions
    {
        writeInstruction("mov\t$" + ins.arg2 + ", %eax");

        int tmpOffset = this->assemblyContext.context.stackOffset;
        this->assemblyContext.context.stackOffset -= 4;
        writeInstruction("sub\t$4, %rsp");
        writeInstruction("mov\t%eax, " + to_string(tmpOffset) + "(%rbp)");
        writeInstruction("mov\t$" + ins.arg1 + ", %eax");

        if (ins.op == "ADD")
        {
            writeInstruction("add\t" + to_string(tmpOffset) + "(%rbp), %eax");
        }
        else if (ins.op == "SUB")
        {
            writeInstruction("sub\t%eax, " + to_string(tmpOffset) + "(%rbp)");
            writeInstruction("mov\t" + to_string(tmpOffset) + "(%rbp), %eax");
        }
        else if (ins.op == "MUL")
        {
            //insert logic
        }
        else if (ins.op == "DIV")
        {

        }
        else if (ins.op == "EQ")
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
        writeInstruction("mov\t$" + ins.arg2 + ", " + to_string(this->assemblyContext.context.stackOffset) + "(%rbp)");
    }
    else if (ins.op == "RET")
    {
        writeFunctionEpilogue();
    }
    else if (ins.op == "FUNC") // generate a label
    {
        //todo: need to generate function prologue and epilogue
        out << ins.arg1 << ":" << endl;
        writeFunctionPrologue();
    }
    else if (ins.op == "LABEL")
    {
        out << ins.arg1 << ":" << endl;
    }
}