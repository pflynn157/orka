#pragma once

#include <string>
#include <vector>

class AMD64File;
class AMD64Instr;
class AMD64Operand;
class AMD64Reg;
class AMD64Const32;

enum class AMD64_Instr {
    Func,
    Label,
    Mov,
    Add,
    Ret
};

enum class AMD64_Operand {
    None,
    Const32,
    Reg32
};

enum class AMD64_R32 {
    EAX,
    EBX,
    ECX,
    EDX
};

//
// Represents a file
//
class AMD64File {
public:
    explicit AMD64File(std::string name) {
        this->name = name;
    }
    
    void addText(AMD64Instr *instr) {
        text.push_back(instr);
    }
    
    std::string getName() {
        return name;
    }
    
    std::vector<AMD64Instr *> getText() {
        return text;
    }
private:
    std::string name = "";
    std::vector<AMD64Instr *> text;
    // TODO: Data
};

//
// Represents an assembly instruction
//
class AMD64Instr {
public:
    explicit AMD64Instr(AMD64_Instr type) {
        this->type = type;
    }
    
    explicit AMD64Instr(AMD64_Instr type, AMD64Operand *op1, AMD64Operand *op2) {
        this->type = type;
        this->op1 = op1;
        this->op2 = op2;
    }
    
    explicit AMD64Instr(AMD64_Instr type, std::string label) {
        this->type = type;
        this->label = label;
    }
    
    void setOperand1(AMD64Operand *op1) {
        this->op1 = op1;
    }
    
    void setOperand2(AMD64Operand *op2) {
        this->op2 = op2;
    }
    
    void setLabel(std::string label) {
        this->label;
    }
    
    AMD64_Instr getType() {
        return this->type;
    }
    
    AMD64Operand *getOperand1() {
        return this->op1;
    }
    
    AMD64Operand *getOperand2() {
        return this->op2;
    }
    
    std::string getLabel() {
        return label;
    }
private:
    AMD64_Instr type;
    AMD64Operand *op1, *op2;
    std::string label;
};

//
// Represents a function
//
class AMD64Function : public AMD64Instr {
public:
    explicit AMD64Function(std::string name, int stackSize) : AMD64Instr(AMD64_Instr::Func, name) {
        this->stackSize = stackSize;
    }
    
    int getStackSize() {
        return this->stackSize;
    }
private:
    int stackSize = 0;
};

// Represents an assembly operand
class AMD64Operand {
public:
    explicit AMD64Operand(AMD64_Operand type) {
        this->type = type;
    }
private:
    AMD64_Operand type = AMD64_Operand::None;
};
