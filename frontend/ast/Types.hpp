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
    Else,
    While,
    End,
    
    Add,
    Sub,
    Mul,
    Div,
    
    GT,
    LT,
    
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

