#pragma once

#include <string>
#include <vector>

#include <LLIR/Type.hpp>

class Module;            // Represents a translation unit
class Function;          // Represents a function within a translation unit
class GlobalConstant;    // Represents a global constant (ie, string, float, etc)
class Block;             // Represents a block of code within a function
class Value;             // Represents an operand or a result in most cases (the base of 3-addr SSA)
class Instruction;       // Represents an assembly instruction

enum class LinkageType {
    None,
    Standard,
    Public,
    Extern
};

enum class GConstType {
    None,
    String,
    F32,
    F64
};

enum class ValueType {
    None,
    Instr,
    Reg,
    I8,
    I16,
    I32,
    I64,
    F32,
    F64
};

enum class InstrType {
    None,
    
    LdI,
    
    Add,
    
    RetVoid,
    Ret
};

//
// Module
// Represents the translation unit
//
class Module {
public:
    explicit Module(std::string name) {
        this->name = name;
    }
    
    void addFunction(Function *func) {
        functions.push_back(func);
    }
    
    std::string getName() {
        return name;
    }
    
    std::vector<Function *> getFunctions() {
        return functions;
    }
    
    std::vector<GlobalConstant *> getGlobalConstants() {
        return globalConstants;
    }
    
    void dump();
private:
    std::string name = "";
    std::vector<Function *> functions;
    std::vector<GlobalConstant *> globalConstants;
};

//
// Function
// Represents a function within a module
//
class Function {
public:
    explicit Function(std::string name, FunctionType *type, LinkageType linkType = LinkageType::Standard) {
        this->name = name;
        this->type = type;
        this->linkType = linkType;
    }
    
    void addBlock(Block *block) {
        blocks.push_back(block);
    }
    
    void dump();
private:
    std::string name = "";
    FunctionType *type = nullptr;
    LinkageType linkType = LinkageType::Standard;
    std::vector<Block *> blocks;
};

//
// Block
// Represents a block within a function
//
class Block {
public:
    explicit Block(std::string name) {
        this->name = name;
    }
    
    void addInstruction(Instruction *i) {
        instructions.push_back(i);
    }
    
    void dump();
private:
    std::string name = "";
    std::vector<Instruction *> instructions;
};

//
// Value
// Represents a value, used for operands/dest in instructions
//
class Value {
public:
    explicit Value(ValueType valueType, Type *type, std::string name) {
        this->valueType = valueType;
        this->type = type;
        this->name = name;
    }
    
    ValueType getValueType() {
        return valueType;
    }
    
    std::string getName() {
        return name;
    }
    
    virtual void dump(bool nl = false) {}
protected:
    std::string name = "";
    Type *type = nullptr;
    ValueType valueType = ValueType::None;
};

class ConstI32 : public Value {
public:
    explicit ConstI32(int val) : Value(ValueType::I32, new I32Type(), "") {
        this->val = val;
    }
    
    int getValue() {
        return val;
    }
    
    void dump(bool nl = false) override;
private:
    int val = 0;
};

//
// Instruction
// Represents an instruction
//
class Instruction : public Value {
public:
    explicit Instruction(InstrType instrType, Type *type, std::string name, bool voidInstr = false)
        : Value(ValueType::Instr, type, name) {
        this->instrType = instrType;
        this->voidInstr = voidInstr;
    }
    
    bool isVoidInstr() {
        return this->voidInstr;
    }
    
    void setOperand(Value *val, int pos) {
        switch (pos) {
            case 1: this->src1 = val; break;
            case 2: this->src2 = val; break;
            case 3: this->src3 = val; break;
        }
    }
    
    Value *getOperand(Value *val, int pos) {
        switch (pos) {
            case 1: return this->src1;
            case 2: return this->src2;
            case 3: return this->src3;
        }
        return nullptr;
    }
    
    void dump(bool nl = true) override;
private:
    InstrType instrType = InstrType::None;
    bool voidInstr = false;
    Value *src1 = nullptr;
    Value *src2 = nullptr;
    Value *src3 = nullptr;
};
