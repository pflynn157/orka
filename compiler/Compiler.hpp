#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

#include <string>
#include <map>
#include <stack>

#include <ast.hpp>

struct CFlags {
    std::string name;
};

class Compiler {
public:
    explicit Compiler(AstTree *tree, CFlags flags);
    void compile();
    void debug();
    void writeAssembly();
    void assemble();
    void link();
protected:
    void compileStatement(AstStatement *stmt);
    Value *compileValue(AstExpression *expr, DataType dataType = DataType::Void);
    Type *translateType(DataType dataType, DataType subType = DataType::Void);

    // Function.cpp
    void compileFunction(AstGlobalStatement *global);
    void compileExternFunction(AstGlobalStatement *global);
    void compileFuncCallStatement(AstStatement *stmt);
    void compileReturnStatement(AstStatement *stmt);
    
    // Flow.cpp
    void compileIfStatement(AstStatement *stmt);
    void compileWhileStatement(AstStatement *stmt);
    void compileRepeatStatement(AstStatement *stmt);
    void compileForStatement(AstStatement *stmt);
    void compileForEachStatement(AstForStmt *stmt);
private:
    AstTree *tree;
    CFlags cflags;

    // LLVM stuff
    std::unique_ptr<LLVMContext> context;
    std::unique_ptr<Module> mod;
    std::unique_ptr<IRBuilder<>> builder;
    Function *currentFunc;
    DataType currentFuncType = DataType::Void;
    
    // The array types
    StructType *i8ArrayType;
    StructType *i32ArrayType;
    StructType *i64ArrayType;
    
    // Symbol table
    std::map<std::string, AllocaInst *> symtable;
    std::map<std::string, DataType> typeTable;
    std::map<std::string, DataType> ptrTable;
    
    // Block stack
    int blockCount = 0;
    std::stack<BasicBlock *> breakStack;
    std::stack<BasicBlock *> continueStack;
};

