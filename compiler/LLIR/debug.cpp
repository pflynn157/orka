#include <iostream>

#include <LLIR/Base.hpp>

std::string Type::toString() {
    switch (type) {
        case LLIRType::I8: return "i8";
        case LLIRType::I16: return "i16";
        case LLIRType::I32: return "i32";
        case LLIRType::I64: return "i64";
        case LLIRType::F32: return "f32";
        case LLIRType::F64: return "f64";
        
        default: return "void";
    }
    return "void";
}

void Module::dump() {
    std::cout << ";" << name << std::endl << std::endl;
    
    // TODO: Constants
    
    // Functions
    for (Function *func : functions) func->dump();
}

void Function::dump() {
    Type *retType = type->getReturnType();
    std::vector<Type *> typeArgs = type->getArgTypes();
    
    std::cout << retType->toString() << " " << name << "(";
    for (int i = 0; i<typeArgs.size(); i++) {
        std::cout << typeArgs.at(i)->toString();
        if (i+1 < typeArgs.size()) {
            std::cout << ", ";
        }
    }
    std::cout << ") {" << std::endl;
    
    // TODO: Blocks
    for (Block *block : blocks) block->dump();
    
    std::cout << "}" << std::endl << std::endl;
}

void Block::dump() {
    std::cout << name << ":" << std::endl;
    for (Instruction *instr : instructions) {
        std::cout << "  ";
        instr->dump();
    }
}

void ConstI32::dump(bool nl) {
    std::cout << val << " ";
    if (nl) std::cout << std::endl;
}

void Instruction::dump(bool nl) {
    if (!voidInstr) {
        std::cout << "%" << name << " = ";
    }

    switch (instrType) {
        case InstrType::LdI: std::cout << "li "; break;
        case InstrType::Add: std::cout << "add "; break;
        case InstrType::Ret: std::cout << "ret "; break;
        case InstrType::RetVoid: std::cout << "ret.void "; break;
        default: std::cout << "<NULL> ";
    }
    
    if (type == nullptr) std::cout << "void ";
    else std::cout << type->toString() << " ";
    
    if (src1 != nullptr) {
        if (src1->getValueType() == ValueType::Instr) {
            std::cout << "%" << src1->getName();
        } else {
            src1->dump();
        }
    }
    if (src2 != nullptr) {
        std::cout << ", ";
        if (src2->getValueType() == ValueType::Instr) std::cout << "%" << src2->getName();
        else src2->dump();
    }
    if (src3 != nullptr) {
        std::cout << ", ";
        if (src3->getValueType() == ValueType::Instr) std::cout << "%" << src3->getName();
        else src3->dump();
    }
    if (nl) std::cout << std::endl;
}
