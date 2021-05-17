#pragma once

#include <fstream>

#include <ltac.hpp>

class Amd64Writer {
public:
    void write(LtacFile *file);
private:
    std::ofstream writer;
    LtacFile *file;
    
    void writeInstr(LtacInstr *instr);
    void writeOperand(LtacOperand *op);
};

