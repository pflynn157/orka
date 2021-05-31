#pragma once

#include <string>
#include <vector>

enum class AstType {
    EmptyAst,
    Func,
    Return,
    
    IntL
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

class AstReturnStmt : public AstStatement {
public:
    explicit AstReturnStmt() : AstStatement(AstType::Return) {}
    void print();
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
private:
    AstType type = AstType::EmptyAst;
};

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

