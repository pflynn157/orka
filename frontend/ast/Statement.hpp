#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>

class AstExpression;
class AstID;

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
    
    void clearExpressions() {
        expressions.clear();
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
    
    void setDataType(DataType dataType) { this->dataType = dataType; }
    void setPtrType(DataType dataType) { this->ptrType = dataType; }
    void setPtrSize(AstExpression *size) { this->size = size; }
    
    std::string getName() { return name; }
    DataType getDataType() { return dataType; }
    DataType getPtrType() { return ptrType; }
    AstExpression *getPtrSize() { return size; }
    
    void print();
private:
    std::string name = "";
    AstExpression *size = nullptr;
    DataType dataType = DataType::Void;
    DataType ptrType = DataType::Void;
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

// Represents an array assignment
class AstArrayAssign : public AstStatement {
public:
    explicit AstArrayAssign(std::string name) : AstStatement(AstType::ArrayAssign) {
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

// Represents an infinite loop statement
class AstLoopStmt : public AstStatement {
public:
    explicit AstLoopStmt() : AstStatement(AstType::Loop) {
        block = new AstBlock;
    }
    
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    AstBlock *getBlock() { return block; }
    
    void print();
private:
    AstBlock *block;
};

// Represents a for loop
class AstForStmt : public AstStatement {
public:
    explicit AstForStmt() : AstStatement(AstType::For) {
        block = new AstBlock;
    }
    
    void setIndex(AstID *indexVar) { this->indexVar = indexVar; }
    void setStartBound(AstExpression *expr) { startBound = expr; }
    void setEndBound(AstExpression *expr) { endBound = expr; }
    void addStatement(AstStatement *stmt) { block->addStatement(stmt); }
    
    AstID *getIndex() { return indexVar; }
    AstExpression *getStartBound() { return startBound; }
    AstExpression *getEndBound() { return endBound; }
    AstBlock *getBlock() { return block; }
    
    void print();
private:
    AstBlock *block;
    AstID *indexVar;
    AstExpression *startBound, *endBound;
};

// Represents a break statement for a loop
class AstBreak : public AstStatement {
public:
    explicit AstBreak() : AstStatement(AstType::Break) {}
    void print();
};

// Represents a continue statement for a loop
class AstContinue : public AstStatement {
public:
    explicit AstContinue() : AstStatement(AstType::Continue) {}
    void print();
};

// Represents a block end
class AstEnd : public AstStatement {
public:
    explicit AstEnd() : AstStatement(AstType::End) {}
    void print();
};

