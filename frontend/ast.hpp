#pragma once

#include <string>
#include <vector>

enum class AstType {
    EmptyAst,
    ExternFunc,
    Func,
    Return,
    
    FuncCallStmt,
    FuncCall,
    
    VarDec,
    VarAssign,
    
    Add,
    Sub,
    Mul,
    Div,
    
    IntL,
    StringL,
    ID
};

enum class DataType {
    Void,
    Char,
    Int32,
    Ptr
};

struct Var {
    std::string name;
    DataType type;
    DataType subType;
};

// Forward declarations
class AstGlobalStatement;
class AstStatement;
class AstExpression;

// Represents an AST tree
class AstTree {
public:
    explicit AstTree(std::string file) { this-> file = file; }
    ~AstTree() {}
    
    std::vector<AstGlobalStatement *> getGlobalStatements() {
        return global_statements;
    }
    
    void addGlobalStatement(AstGlobalStatement *stmt) {
        global_statements.push_back(stmt);
    }
    
    void print();
private:
    std::string file = "";
    std::vector<AstGlobalStatement *> global_statements;
};

// Represents a function, external declaration, or global variable
class AstGlobalStatement {
public:
    explicit AstGlobalStatement() {}
    explicit AstGlobalStatement(AstType type) {
        this->type = type;
    }
    
    AstType getType() { return type; }
    
    virtual void print() {}
private:
    AstType type = AstType::EmptyAst;
};

// Represents an extern function
class AstExternFunction : public AstGlobalStatement {
public:
    explicit AstExternFunction(std::string name) : AstGlobalStatement(AstType::ExternFunc) {
        this->name = name;
    }
    
    void setArguments(std::vector<Var> args) { this->args = args; }
    
    std::string getName() { return name; }
    std::vector<Var> getArgs() { return args; }
    void print() override;
private:
    std::string name = "";
    std::vector<Var> args;
};

// Represents a function
class AstFunction : public AstGlobalStatement {
public:
    explicit AstFunction(std::string name) : AstGlobalStatement(AstType::Func) {
        this->name = name;
    }
    
    std::string getName() { return name; }
    std::vector<AstStatement *> getCode() { return code; }
    
    void addStatement(AstStatement *statement) {
        code.push_back(statement);
    }
    
    void print() override;
private:
    std::string name = "";
    std::vector<AstStatement *> code;
};

// Represents an AST statement
class AstStatement {
public:
    explicit AstStatement() {}
    explicit AstStatement(AstType type) {
        this->type = type;
    }
    
    void addExpression(AstExpression *expr) {
        expressions.push_back(expr);
    }
    
    int getExpressionCount() {
        return expressions.size();
    }
    
    std::vector<AstExpression *> getExpressions() { return expressions; }
    AstType getType() { return type; }
    virtual void print() {}
private:
    std::vector<AstExpression *> expressions;
    AstType type = AstType::EmptyAst;
};

// Represents a function call statement
class AstFuncCallStmt : public AstStatement {
public:
    explicit AstFuncCallStmt(std::string name) : AstStatement(AstType::FuncCallStmt) {
        this->name = name;
    }
    
    std::string getName() { return name; }
    void print();
private:
    std::string name = "";
};

// Represents a return statement
class AstReturnStmt : public AstStatement {
public:
    explicit AstReturnStmt() : AstStatement(AstType::Return) {}
    void print();
};

// Represents a variable declaration
class AstVarDec : public AstStatement {
public:
    explicit AstVarDec(std::string name, DataType dataType) : AstStatement(AstType::VarDec) {
        this->name = name;
        this->dataType = dataType;
    }
    
    std::string getName() { return name; }
    DataType getDataType() { return dataType; }
    
    void print();
private:
    std::string name = "";
    DataType dataType = DataType::Void;
};

// Represents a variable assignment
class AstVarAssign : public AstStatement {
public:
    explicit AstVarAssign(std::string name) : AstStatement(AstType::VarAssign) {
        this->name = name;
    }
    
    std::string getName() { return name; }
    
    void print();
private:
    std::string name = "";
};

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

