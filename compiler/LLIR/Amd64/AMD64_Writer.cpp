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
            
            case AMD64_Instr::Mov: {
                AMD64Mov *mov = static_cast<AMD64Mov *>(instr);
            
                writer << "  mov ";
                writeOperand(mov->getOperand1());
                writer << ", ";
                writeOperand(mov->getOperand2());
                writer << std::endl;
            } break;
            
            case AMD64_Instr::Add: {
                AMD64Add *add = static_cast<AMD64Add *>(instr);
                
                writer << "  add ";
                writeOperand(add->getOperand1());
                writer << ", ";
                writeOperand(add->getOperand2());
                writer << std::endl;
            } break;
            
            case AMD64_Instr::Ret: {
                writer << "  leave" << std::endl;
                writer << "  ret" << std::endl;
                writer << std::endl;
            } break;
            
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

void AMD64Writer::writeOperand(AMD64Operand *op) {
    switch (op->getType()) {
        case AMD64_Operand::Const32: {
            AMD64Const32 *const32 = static_cast<AMD64Const32 *>(op);
            writer << const32->getValue();
        } break;
        
        case AMD64_Operand::Reg32: {
            AMD64Reg32 *reg32 = static_cast<AMD64Reg32 *>(op);
            switch (reg32->getRegType()) {
                case AMD64_R32::EAX: writer << "eax"; break;
                case AMD64_R32::EBX: writer << "ebx"; break;
                case AMD64_R32::ECX: writer << "ecx"; break;
                case AMD64_R32::EDX: writer << "edx"; break;
            }
        } break;
    }
}
