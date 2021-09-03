// The definition for portable assembly IR (PASM)
#pragma once

#include <string>
#include <vector>

class PASMFile;
class PASMInstruction;
class PASMOperand;

enum class PASM {
    None,
    
    // Instructions
    NOP,
    LI,
    RET,
    
    // Operands
    R8,
    R16,
    R32,
    R64,
    Imm
};

//
// Represents a portable assembly file
//
class PASMFile {
public:
    explicit PASMFile(std::string name) {
        this->name = name;
    }
    
    void addCode(PASMInstruction *instr) {
        code.push_back(instr);
    }
    
    std::vector<PASMInstruction *> getCode() {
        return code;
    }
private:
    std::string name = "";
    std::vector<PASMInstruction *> code;
};

//
// Represents an instruction
//
class PASMInstruction {
public:
    explicit PASMInstruction(PASM opType) {
        this->opType = opType;
    }
    
    void setOperand1(PASMOperand *op) { op1 = op; }
    void setOperand2(PASMOperand *op) { op2 = op; }
    void setOperand3(PASMOperand *op) { op3 = op; }
    
    PASMOperand *getOperand1() { return op1; }
    PASMOperand *getOperand2() { return op2; }
    PASMOperand *getOperand3() { return op3; }
protected:
    PASM opType = PASM::None;
    PASMOperand *op1 = nullptr;
    PASMOperand *op2 = nullptr;
    PASMOperand *op3 = nullptr;
};

//
// Represents an operand
//
class PASMOperand {
public:
    explicit PASMOperand(PASM opType) {
        this->opType = opType;
    }
    
private:
    PASM opType = PASM::None;
};

