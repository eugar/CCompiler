cmake_minimum_required(VERSION 3.13)
project(CCompiler)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_executable(CCompiler
        ccompiler.hpp
        ccompiler.cpp
        cexpression.cpp
        cexpression.hpp
        cscanner.cpp
        cscanner.hpp grammartree.cpp grammartree.h definitions.h grammar.h grammar.cpp statetablegenerator.cpp statetablegenerator.h symboltable.cpp symboltable.hpp token.cpp token.h statetables.cpp statetables.h ir.cpp ir.h)
