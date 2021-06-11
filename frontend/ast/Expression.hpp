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

// Represents an equal-to operation
class AstEQOp : public AstBinaryOp {
public:
    AstEQOp() {
        this->type = AstType::EQ;
    }
    
    void print();
};

// Represents a not-equal-to operation
class AstNEQOp : public AstBinaryOp {
public:
    AstNEQOp() {
        this->type = AstType::NEQ;
    }
    
    void print();
};

// Represents a greater-than operation
class AstGTOp : public AstBinaryOp {
public:
    AstGTOp() {
        this->type = AstType::GT;
    }
    
    void print();
};

// Represents a less-than operation
class AstLTOp : public AstBinaryOp {
public:
    AstLTOp() {
        this->type = AstType::LT;
    }
    
    void print();
};

// Represents a greater-than-or-equal operation
class AstGTEOp : public AstBinaryOp {
public:
    AstGTEOp() {
        this->type = AstType::GTE;
    }
    
    void print();
};

// Represents a less-than-or-equal operation
class AstLTEOp : public AstBinaryOp {
public:
    AstLTEOp() {
        this->type = AstType::LTE;
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

// Represents the sizeof operator
class AstSizeof : public AstExpression {
public:
    explicit AstSizeof(AstID *val) : AstExpression(AstType::Sizeof) {
        this->val = val;
    }
    
    AstID *getValue() { return val; }
    void print();
private:
    AstID *val;
};

// Represents an array access
class AstArrayAccess : public AstExpression {
public:
    explicit AstArrayAccess(std::string val) : AstExpression(AstType::ArrayAccess) {
        this->val = val;
    }
    
    void setIndex(AstExpression *index) { this->index = index; }
    
    std::string getValue() { return val; }
    AstExpression *getIndex() { return index; }
    void print();
private:
    std::string val = "";
    AstExpression *index;
};

// Represents a function call
class AstFuncCallExpr : public AstExpression {
public:
    explicit AstFuncCallExpr(std::string name) : AstExpression(AstType::FuncCallExpr) {
        this->name = name;
    }
    
    void setArguments(std::vector<AstExpression *> args) {
        this->args = args;
    }
    
    void addArgument(AstExpression *arg) { args.push_back(arg); }
    void clearArguments() { args.clear(); }
    
    std::vector<AstExpression *> getArguments() { return args; }
    std::string getName() { return name; }
    void print();
private:
    std::vector<AstExpression *> args;
    std::string name = "";
};

