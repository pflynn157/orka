#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>

class AstStatement;

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
    std::vector<Var> getArguments() { return args; }
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
        block = new AstBlock;
    }
    
    std::string getName() { return name; }
    DataType getDataType() { return dataType; }
    std::vector<Var> getArguments() { return args; }
    AstBlock *getBlock() { return block; }
    
    void setArguments(std::vector<Var> args) { this->args = args; }
    
    void addStatement(AstStatement *statement) {
        block->addStatement(statement);
    }
    
    void setDataType(DataType dataType) { this->dataType = dataType; }
    
    void print() override;
private:
    std::string name = "";
    std::vector<Var> args;
    AstBlock *block;
    DataType dataType = DataType::Void;
};

