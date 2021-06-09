#pragma once

#include <string>

enum class AstType {
    EmptyAst,
    ExternFunc,
    Func,
    Return,
    
    FuncCallStmt,
    FuncCall,
    
    VarDec,
    VarAssign,
    
    If,
    Elif,
    Else,
    While,
    For,
    End,
    
    Break,
    Continue,
    
    Add,
    Sub,
    Mul,
    Div,
    
    EQ,
    NEQ,
    GT,
    LT,
    GTE,
    LTE,
    
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

// Represents a block
class AstStatement;

class AstBlock {
public:
    void addStatement(AstStatement *stmt) { block.push_back(stmt); }
    void addStatements(std::vector<AstStatement *> block) { this->block = block; }
    std::vector<AstStatement *> getBlock() { return block; }
private:
    std::vector<AstStatement *> block;
};

