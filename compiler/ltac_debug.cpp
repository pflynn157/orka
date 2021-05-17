#include <iostream>

#include <ltac.hpp>

void LtacFile::print() {
    std::cout << "LTAC: " << output << std::endl;
    std::cout << std::endl;
    
    for (auto ln : code) ln->print();
}

void LtacInstr::print() {
    if (type != LtacType::Func) std::cout << "  ";

    switch (type) {
        case LtacType::Nop: std::cout << "nop "; break;
        case LtacType::Func: std::cout << "func "; break;
        case LtacType::Return: std::cout << "ret "; break;
        
        case LtacType::Li: std::cout << "li "; break;
    }
    
    if (label != "") std::cout << label;
    
    if (op1 != nullptr) op1->print();
    if (op2 != nullptr) {
        std::cout << ", ";
        op2->print();
    }
    if (op3 != nullptr) {
        std::cout << ", ";
        op3->print();
    }
    std::cout << std::endl;
}

void LtacOperand::print() {
    switch (opType) {
        case LtacType::RetReg32: std::cout << "r32.ret"; break;
        case LtacType::OpReg32: std::cout << "r32.reg" << pos; break;
        case LtacType::Int32: std::cout << i32_val; break;
    }
}

