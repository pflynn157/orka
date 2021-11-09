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
private:
    LLIRType type;
};

class FunctionType {
public:
    explicit FunctionType(Type *retType, std::vector<Type *> argTypes) {
        this->retType = retType;
        this->argTypes = argTypes;
    }
    
private:
    Type *retType;
    std::vector<Type *> argTypes;
};