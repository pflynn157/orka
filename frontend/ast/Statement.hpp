#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>

class AstExpression;

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

