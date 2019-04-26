//
// Created by eugene on 2019-04-11.
//

#include <iostream>
#include <algorithm>
#include "assembly.h"
#include <algorithm>

using namespace std;

Assembly::Assembly(vector<irInstruction> instructions, string filename) {
    this->out.open(filename);
    this->instructions = instructions;

    if (!out.is_open())
    {
        cout << "File " << filename << " could not be opened to write the assembly code file." << endl;
        exit(1);
    }

    bbcount = 1;
    insCount = 0;
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
    out << ".section .data" << endl;
    out << ".section .bss" << endl;
    out << ".section .text" << endl;
    out << ".globl _start" << endl;
    out << "#.bb0:" << endl;
    out << "_start:" << endl;
    writeFunctionPrologue();
    out << "" << endl;
    writeInstruction("call\t\tmain"); // need to do a check to make sure that main exist
    writeInstruction("movq\t\t%rax, %rbx");
    writeInstruction("movq\t\t$60, %rax #sys_exit ");
    writeInstruction("movq\t\t$0x0, %rdi");
    writeInstruction("syscall");
}

void Assembly::generateCode(vector<irInstruction> instructions)
{
    writeHeader();

    for (auto instruction : instructions)
    {
        insertBB(instruction);
        this->insCount++;
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

int Assembly::getNextOffset(string argument) {
    if (argument.find_first_not_of("0123456789") != std::string::npos) // a char was found, it is an id
    {
        //lookup in the table
        int offset = this->assemblyContext.getOffset(argument);
        if (offset != -1) // it was found
        {
            return offset;
        }
    }
    this->assemblyContext.context.stackOffset -= 4;
    return this->assemblyContext.context.stackOffset;
}

int Assembly::countLocalVars() {
    int tmp = this->insCount + 1;

    vector<irInstruction> localVars;

    while (this->instructions.at(tmp).op != "RET" && tmp < this->instructions.size()) // possible off by one
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
            }
        }
    tmp++;
    }

    return localVars.size() * 4; //assumes ints
}

string Assembly::createString(string argument)
{
    if (argument.find_first_not_of("0123456789") != std::string::npos) // a char was found, it is an id
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
void Assembly::chooseInstruction(irInstruction ins)
{
    if (ins.op == "NOT") // unary instructions
    {
        writeInstruction(createString(ins.arg1));
        writeInstruction("not\t%eax"); // need to change logic here
        writeInstruction("movl\t%eax, %r" + to_string(getNextOffset(ins.res)));
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
        {
            writeInstruction("movl\t\t" + createString(ins.arg2) + ", %eax");
            writeInstruction("cdq");
            writeInstruction("idivl\t\t" + createString(ins.arg1));
        }
        int tmp = getNextOffset(ins.res);
        writeInstruction("movl\t\t%eax, " + to_string(tmp) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, tmp);
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
        int offset = getNextOffset(ins.res);
        writeInstruction("movl\t\t" + createString(ins.arg1) + ", " + to_string(offset) + "(%rbp)");
        this->assemblyContext.setOffset(ins.res, offset);
    }
    else if (ins.op == "RET")
    {
        int tmp = this->assemblyContext.getOffset(ins.arg1);

        if (tmp != -1)
        {
            writeInstruction("movl\t\t" + to_string(tmp) + "(%rbp), %eax"); // move the return value in to the return register
            writeFunctionEpilogue();
        }
        else
        {
            cout << "return value not found" << endl;
            exit(1);
        }
    }
    else if (ins.op == "FUNC") // generate a label
    {
        //todo: need to insert logic to handle scoping
        out << ins.res << ":" << endl;
        writeFunctionPrologue();
        writeInstruction("subl\t\t$" + to_string(countLocalVars()) + ", %rsp"); // make room for local vars in this stack frame
        this->assemblyContext.newScope();
    }
    else if (ins.op == "LABEL")
    {
        // same as above
        out << ins.arg1 << ":" << endl;
    }
}
