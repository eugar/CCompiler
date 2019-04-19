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
        updateRegAge();
        /*for (auto reg : this->assemblyContext.context.registerList) // debugging
        {
            cout << reg.registerNum << ':' << reg.age << endl;
        }
        cout << "" << endl;*/
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

reg Assembly::evictReg()
{
    // find 'oldest' register
    auto max = max_element(this->assemblyContext.context.registerList.begin(), this->assemblyContext.context.registerList.end(),
            [](const reg &a, const reg &b)
            {
                return a.age < b.age;
            } );

    max->age = 0;
    max->isUsed = 0;
    max->name = "";
    max->val = 0;

    return *max;
}

reg Assembly::getOpenReg(string res) {
    for (auto &reg : this->assemblyContext.context.registerList) // see if the result destination is already a register
    {
        if (reg.name == res)
        {
            cout << "returning " << reg.registerNum << " " << reg.name << endl;
            return reg;
        }
    }

    for (auto &reg : this->assemblyContext.context.registerList) // otherwise, we need to iterate through the registers and get an open register
    {
        if (!reg.isUsed)
        {
            //cout << "found open reg " << reg.registerNum << endl;
            return reg;
        }
    }

    // if we reached this point, there are no more open registers. we must evict the LRU register.

    reg openReg = evictReg();

    return openReg;
}

// iterate through the list and increment the age of all registers
void Assembly::updateRegAge()
{
    for (auto &reg : this->assemblyContext.context.registerList)
    {
        if (reg.isUsed)
        {
            reg.age += 1;
        }
    }
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
{ //todo: choose appropriate instructions, e.g., addq, addw, etc
    if (ins.op == "NOT") // unary instructions
    {
        reg tmpReg = getOpenReg(ins.res); // need to change logic in this op
        writeInstruction(createString(ins.arg1));
        writeInstruction("not\t%eax");
        writeInstruction("mov\t%eax, %r" + to_string(tmpReg.registerNum));
    }
    else if (ins.op == "ADD" || ins.op == "SUB" || ins.op == "MUL" || ins.op == "DIV")
    {
        writeInstruction("mov\t\t" + createString(ins.arg2) + ", %eax");
        writeInstruction("mov\t\t" + createString(ins.arg1) + ", %ecx");

        if (ins.op == "ADD")
        {
            writeInstruction("add\t\t%ecx, %eax");
        }
        else if (ins.op == "SUB")
        {
            writeInstruction("sub\t\t%ecx, %eax");
        }
        else if (ins.op == "MUL")
        {
            writeInstruction("mul\t\t%ecx, %eax");
        }
        else if (ins.op == "DIV")
        {

        }
        // todo: still need to check where the result is being saved, i.e., is it going back into one of the arguments? need to check where that reg is at
        reg tmpReg = getOpenReg(ins.res);
        writeInstruction("mov\t\t%eax, %r" + to_string(tmpReg.registerNum));
        this->assemblyContext.fillRegister(tmpReg.registerNum, ins.res);
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
