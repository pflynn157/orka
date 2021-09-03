#pragma once

#include <string>
#include <vector>

class LLIRFile;
class LLIRConst;
class LLIRStringConst;
class LLIRFunction;
class LLIRBlock;
class LLIRValue;
class LLIRInstruction;

enum LLIR {
    None,
    
    // Constants
    StrConst,
    
    // Instructions
    NOP,
    Ret,
    RetVoid,
    
    // Values
    Imm
};

//
// Represents LLIR data types for values
//
enum LLIRDataType {
    Void,
    I8,
    I16,
    I32,
    I64,
    Ptr
};

class LLIRType {
public:
    explicit LLIRType(LLIRDataType type) {
        this->type = type;
    }
protected:
    LLIRDataType type = LLIRDataType::Void;
};

//
// Represents an LLIR file
//
class LLIRFile {
public:
    explicit LLIRFile(std::string name) {
        this->name = name;
    }
    
    std::vector<LLIRConst *> getConsts() { return consts; }
    std::vector<LLIRFunction *> getFunctions() { return functions; }
private:
    std::string name = "";
    std::vector<LLIRConst *> consts;
    std::vector<LLIRFunction *> functions;
};

//
// Represents constant values
//
class LLIRConst {
public:
    explicit LLIRConst(LLIR type) {
        this->type = type;
    }
protected:
    LLIR type = LLIR::None;
};

class LLIRStringConst : public LLIRConst {
public:
    explicit LLIRStringConst(std::string name, std::string value) : LLIRConst(LLIR::StrConst) {
        this->name = name;
        this->value = value;
    }
    
    std::string getName() { return name; }
    std::string getValue() { return value; }
private:
    std::string name = "";
    std::string value = "";
};

//
// Represents a function
//
class LLIRFunction {
public:
    explicit LLIRFunction(std::string name, LLIRType *type) {
        this->name = name;
        this->type = type;
    }
    
    void setEntryBlock(LLIRBlock *block) {
        entryBlock = block;
    }
    
    void addBlock(LLIRBlock *block) {
        //TODO
    }
    
    LLIRBlock *getEntryBlock() { return entryBlock; }
    
    std::string getName() { return name; }
    LLIRType *getType() { return type; }
private:
    std::string name = "";
    LLIRType *type = new LLIRType(LLIRDataType::Void);
    LLIRBlock *entryBlock = nullptr;
};

//
// Represents a block, which holds instructions
// In assembly, this will become a label
//
class LLIRBlock {
public:
    explicit LLIRBlock(std::string name) {
        this->name = name;
    }
    
    void addInstruction(LLIRValue *val) {
        code.push_back(val);
    }
    
    std::vector<LLIRValue *> getCode() { return code; }
    std::string getName() { return name; }
private:
    std::string name = "";
    LLIRBlock *next = nullptr;
    std::vector<LLIRValue *> code;
};

//
// The core classes- represents instructions and values in our 3-address SSA
//
class LLIRValue {
public:
    explicit LLIRValue(std::string name, LLIRType *type, LLIR opType) {
        this->name = name;
        this->type = type;
        this->opType = opType;
    }
    
    std::string getName() { return name; }
    LLIRType *getType() { return type; }
    LLIR getOpType() { return opType; }
protected:
    std::string name = "";
    LLIRType *type = nullptr;
    LLIR opType = LLIR::NOP;
};

class LLIRInstruction : public LLIRValue {
public:
    explicit LLIRInstruction(std::string name, LLIRType *type, LLIR opType) : LLIRValue(name, type, opType) {}
    void setArg1(LLIRValue *val) { arg1 = val; }
    void setArg2(LLIRValue *val) { arg2 = val; }
    void setArg3(LLIRValue *val) { arg3 = val; }
    
    LLIRValue *getArg1() { return arg1; }
    LLIRValue *getArg2() { return arg2; }
    LLIRValue *getArg3() { return arg3; }
private:
    LLIRValue *arg1, *arg2, *arg3;
};

