#pragma once

#include <string>

enum class AstType {
    EmptyAst,
    ExternFunc,
    Func,
    Return,
    
    FuncCallStmt,
    FuncCallExpr,
    
    VarDec,
    VarAssign,
    ArrayAssign,
    Sizeof,
    
    If,
    Elif,
    Else,
    While,
    Loop,
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
    
    CharL,
    IntL,
    StringL,
    ID,
    ArrayAccess
};

enum class DataType {
    Void,
    Char,
    Byte,
    Int32,
    String,
    Array,
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

