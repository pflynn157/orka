#include <fstream>
#include <iostream>
#include <string>

#include <Amd64/AMD64_IR.hpp>
#include <Amd64/AMD64_Writer.hpp>

void AMD64Writer::write() {
    std::string path = "/tmp/" + file->getName() + ".asm";
    writer = std::ofstream(path);
    
    writer << ".intel_syntax noprefix" << std::endl;
    writer << std::endl;
    
    // Write the code section
    
    // Write the text section
    writer << ".text" << std::endl;
    
    for (AMD64Instr *instr : file->getText()) {
        switch (instr->getType()) {
            case AMD64_Instr::Func: writeFunc(instr); break;
            
            default: {}
        }
    }
}

void AMD64Writer::writeFunc(AMD64Instr *instr) {
    AMD64Function *func = static_cast<AMD64Function *>(instr);
    
    writer << ".global " << func->getLabel() << std::endl;
    writer << func->getLabel() << ":" << std::endl;
    writer << "  push rbp" << std::endl;
    writer << "  mov rbp, rsp" << std::endl;
}