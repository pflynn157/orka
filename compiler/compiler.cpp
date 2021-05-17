#include <compiler.hpp>

#include <ltac.hpp>

void compile(LLirFile *file, std::string output_path) {

    LtacFile *ltac = new LtacFile(output_path);
    
    // func main
    LtacInstr *instr0 = new LtacInstr(LtacType::Func);
    instr0->label = "main";
    ltac->addCode(instr0);
    
    // li ret.reg, 5
    LtacInstr *instr1 = new LtacInstr(LtacType::Li);
    LtacOperand *op1 = new LtacOperand(LtacType::RetReg32);
    op1->pos = 0;
    LtacOperand *op2 = new LtacOperand(LtacType::Int32);
    op2->i32_val = 5;
    instr1->op1 = op1;
    instr1->op2 = op2;
    ltac->addCode(instr1);
    
    // ret
    LtacInstr *instr2 = new LtacInstr(LtacType::Return);
    ltac->addCode(instr2);
    
    
    ltac->print();
}

