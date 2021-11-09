#pragma once

#include <string>
#include <vector>

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
    
    std::string getName() {
        return name;
    }
    
    std::vector<Function *> getFunctions() {
        return functions;
    }
    
    std::vector<GlobalConstant *> getGlobalConstants() {
        return globalConstants;
    }
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
private:
    std::string name = "";
    Type *type = nullptr;
    ValueType valueType = ValueType::None;
};

//
// Instruction
// Represents an instruction
//
class Instruction {
public:
    explicit Instruction(InstrType type) {
        this->type = type;
    }
private:
    InstrType type = InstrType::None;
};
