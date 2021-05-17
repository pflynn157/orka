#include <fstream>
#include <iostream>

#include <amd64/amd64.hpp>

void Amd64Writer::write(LtacFile *file) {
    writer = std::ofstream(file->getPath());
    this->file = file;
    
    writer << ".intel_syntax noprefix" << std::endl;
    writer << ".data" << std::endl;
    
    writer << ".text" << std::endl;
    for (auto instr : file->getCode()) {
        writeInstr(instr);
    }
}

// The main instruction writer function
void Amd64Writer::writeInstr(LtacInstr *instr) {
    switch (instr->type) {
        case LtacType::Func: {
            writer << ".global " << instr->label << std::endl;
            writer << instr->label << ":" << std::endl;
        } break;
        
        case LtacType::Return: {
            writer << "  ret" << std::endl;
            writer << std::endl;
        } break;
        
        case LtacType::Li: {
            writer << "  mov ";
            writeOperand(instr->op1);
            writer << ", ";
            writeOperand(instr->op2);
            writer << std::endl;
        } break;
    }
}

void Amd64Writer::writeOperand(LtacOperand *op) {
    switch (op->opType) {
        case LtacType::RetReg32: writer << "eax"; break;
        case LtacType::Int32: writer << op->i32_val; break;
    }
}

