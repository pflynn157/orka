#pragma once

#include <string>
#include <vector>
#include <fstream>

enum class LLirType {
    None,
    Return,
    
    Int32
};

// Forward declarations
class LLirFuncDefinition;
class LLirStatement;
class LLirOperand;

// Represents a file for the LLIR
class LLirFile {
public:
    explicit LLirFile(std::string input_file) {
        this->input_file = input_file;
    }
    
    void addFunctionDefinition(LLirFuncDefinition *def) {
        func_defs.push_back(def);
    }
    
    std::vector<LLirFuncDefinition *> getFuncDefinitions() { return func_defs; }
    
    void writeOut(std::string output_path);
private:
    std::string input_file = "";
    std::vector<LLirFuncDefinition *> func_defs;
};

// Represents an LLIR function
class LLirFuncDefinition {
public:
    explicit LLirFuncDefinition(std::string name) {
        this->name = name;
    }
    
    void addCode(LLirStatement *line) {
        main_block.push_back(line);
    }
    
    std::string getName() { return name; }
    std::vector<LLirStatement *> getCode() { return main_block; }
    void writeOut(std::ofstream &writer);
private:
    std::string name = "";
    std::vector<LLirStatement *> main_block;
};

// Represents an LLIR base statement
class LLirStatement {
public:
    explicit LLirStatement(LLirType type) {
        this->type = type;
    }
    
    LLirOperand *getOperand(int pos) {
        switch (pos) {
            case 1: return op1;
            case 2: return op2;
            case 3: return op3;
            default: return op1;
        }
        return op1;
    }
    
    void setOperand(LLirOperand *op, int pos) {
        switch (pos) {
            case 1: op1 = op; break;
            case 2: op2 = op; break;
            case 3: op3 = op; break;
            default: op1 = op;
        }
    }
    
    LLirType getType() { return type; }
    virtual void writeOut(std::ofstream &writer) {}
protected:
    LLirType type = LLirType::None;
    LLirOperand *op1, *op2, *op3;
};

// Represents a return statement
class LLirReturn : public LLirStatement {
public:
    explicit LLirReturn() : LLirStatement(LLirType::Return) {}
    void writeOut(std::ofstream &writer);
};

// Represents an LLIR operand for statements
class LLirOperand {
public:
    explicit LLirOperand(LLirType type) {
        this->type = type;
    }
    
    LLirType getType() { return type; }
    virtual void writeOut(std::ofstream &writer) {}
private:
    LLirType type = LLirType::None;
};

// Represents an integer operand
class LLirInt32 : public LLirOperand {
public:
    explicit LLirInt32(int val) : LLirOperand(LLirType::Int32) {
        this->val = val;
    }
    
    int getValue() { return val; }
    void writeOut(std::ofstream &writer);
private:
    int val = 0;
};

