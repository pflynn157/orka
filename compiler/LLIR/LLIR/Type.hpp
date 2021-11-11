#pragma once

#include <vector>

enum class LLIRType {
    Void,
    I8,
    I16,
    I32,
    I64,
    F32,
    F64
};

class Type {
public:
    explicit Type(LLIRType type) {
        this->type = type;
    }
    
    LLIRType getType() {
        return this->type;
    }
    
    std::string toString();
private:
    LLIRType type;
};

class FunctionType {
public:
    explicit FunctionType(Type *retType, std::vector<Type *> argTypes) {
        this->retType = retType;
        this->argTypes = argTypes;
    }
    
    explicit FunctionType(Type *retType) {
        this->retType = retType;
    }
    
    Type *getReturnType() {
        return retType;
    }
    
    std::vector<Type *> getArgTypes() {
        return argTypes;
    }
private:
    Type *retType;
    std::vector<Type *> argTypes;
};

class VoidType : public Type {
public:
    explicit VoidType() : Type(LLIRType::Void) {}
};

class I32Type : public Type {
public:
    explicit I32Type() : Type(LLIRType::I32) {}
};
