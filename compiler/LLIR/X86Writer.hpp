#pragma once

#include <string>
#include <fstream>

#include <LLIR/PASM.hpp>

class X86Writer {
public:
    explicit X86Writer(PASMFile *file);
    void compile();
private:
    void writeInstruction(PASMInstruction *line);
    void writeOperand(PASMOperand *op);

    PASMFile *file = nullptr;
    std::string outputPath;
    std::ofstream writer;
};

