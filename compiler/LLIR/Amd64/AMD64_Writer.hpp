#pragma once

#include <fstream>

#include <Amd64/AMD64_IR.hpp>

class AMD64Writer {
public:
    explicit AMD64Writer(AMD64File *file) {
        this->file = file;
    }
    
    void write();
protected:
    void writeFunc(AMD64Instr *instr);
    void writeOperand(AMD64Operand *op);
private:
    std::ofstream writer;
    AMD64File *file = nullptr;
};