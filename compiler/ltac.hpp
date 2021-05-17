#pragma once

#include <string>
#include <vector>

// Represents LTAC types
enum class LtacType {
    Nop,
    
    Func,
    Return,
    
    Li,
    
    // Operand types
    RetReg32,
    OpReg32,
    
    Int32
};

// Forward declarations
class LtacInstr;
class LtacOperand;

// Represents an LTAC file
class LtacFile {
public:
    explicit LtacFile(std::string output) {
        this->output = output;
    }
    
    void addCode(LtacInstr *instr) {
        code.push_back(instr);
    }
    
    std::vector<LtacInstr *> getCode() { return code; }
    void print();
private:
    std::string output = "";
    std::vector<LtacInstr *> code;
};

// Represents an LTAC instruction
struct LtacInstr {
    LtacInstr(LtacType type) { this->type = type; }

    LtacType type = LtacType::Nop;
    LtacOperand *op1 = nullptr;
    LtacOperand *op2 = nullptr;
    LtacOperand *op3 = nullptr;
    
    std::string label = "";
    
    void print();
};

// Represents an LTAC operand
struct LtacOperand {
    LtacOperand(LtacType type) { this->opType = type; }
    
    LtacType opType;
    int pos;
    int i32_val;
    
    void print();
};

