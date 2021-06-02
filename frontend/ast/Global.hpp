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

