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

// Represents a conditional statement
class AstIfStmt : public AstStatement {
public:
    explicit AstIfStmt() : AstStatement(AstType::If) {
        block = new AstBlock;
    }
    
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    void addBranch(AstStatement *stmt) { branches.push_back(stmt); }
    AstBlock *getBlock() { return block; }
    std::vector<AstStatement *> getBranches() { return branches; }
    
    void print();
private:
    AstBlock *block;
    std::vector<AstStatement *> branches;
};

class AstElifStmt : public AstStatement {
public:
    explicit AstElifStmt() : AstStatement(AstType::Elif) {
        block = new AstBlock;
    }
    
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    AstBlock *getBlock() { return block; }
    
    void print();
private:
    AstBlock *block;
};

class AstElseStmt : public AstStatement {
public:
    explicit AstElseStmt() : AstStatement(AstType::Else) {
        block = new AstBlock;
    }
    
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    AstBlock *getBlock() { return block; }
    
    void print();
private:
    AstBlock *block;
};

// Represents a while statement
class AstWhileStmt : public AstStatement {
public:
    explicit AstWhileStmt() : AstStatement(AstType::While) {
        block = new AstBlock;
    }
    
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    AstBlock *getBlock() { return block; }
    
    void print();
private:
    AstBlock *block;
};

// Represents a block end
class AstEnd : public AstStatement {
public:
    explicit AstEnd() : AstStatement(AstType::End) {}
    void print();
};

