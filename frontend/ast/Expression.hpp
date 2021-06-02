#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>

// Represents an AST expression
class AstExpression {
public:
    explicit AstExpression() {}
    explicit AstExpression(AstType type) {
        this->type = type;
    }
    
    AstType getType() { return type; }
    virtual void print() {}
protected:
    AstType type = AstType::EmptyAst;
};

// Represents the base of a binary expression
class AstBinaryOp : public AstExpression {
public:
    void setLVal(AstExpression *lval) { this->lval = lval; }
    void setRVal(AstExpression *rval) { this->rval = rval; }
    
    AstExpression *getLVal() { return lval; }
    AstExpression *getRVal() { return rval; }
    
    virtual void print() {}
protected:
    AstExpression *lval, *rval;
};

// Represents an add operation
class AstAddOp : public AstBinaryOp {
public:
    AstAddOp() {
        this->type = AstType::Add;
    }
    
    void print();
};

// Represents a subtraction operation
class AstSubOp : public AstBinaryOp {
public:
    AstSubOp() {
        this->type = AstType::Sub;
    }
    
    void print();
};

// Represents a multiplication operation
class AstMulOp : public AstBinaryOp {
public:
    AstMulOp() {
        this->type = AstType::Mul;
    }
    
    void print();
};

// Represents a division operation
class AstDivOp : public AstBinaryOp {
public:
    AstDivOp() {
        this->type = AstType::Div;
    }
    
    void print();
};

// Represents an integer literal
class AstInt : public AstExpression {
public:
    explicit AstInt(int val) : AstExpression(AstType::IntL) {
        this->val = val;
    }
    
    int getValue() { return val; }
    void print();
private:
    int val = 0;
};

// Represents a string literal
class AstString : public AstExpression {
public:
    explicit AstString(std::string val) : AstExpression(AstType::StringL) {
        this->val = val;
    }
    
    std::string getValue() { return val; }
    void print();
private:
    std::string val = "";
};

// Represents a variable reference
class AstID: public AstExpression {
public:
    explicit AstID(std::string val) : AstExpression(AstType::ID) {
        this->val = val;
    }
    
    std::string getValue() { return val; }
    void print();
private:
    std::string val = "";
};

