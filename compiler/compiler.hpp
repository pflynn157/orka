#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

#include <ast.hpp>

class Compiler {
public:
    explicit Compiler(AstTree *tree);
    void compile();
    void debug();
protected:
    void compileStatement(AstStatement *stmt);
    Value *compileValue(AstExpression *expr);
private:
    AstTree *tree;

    // LLVM stuff
    std::unique_ptr<LLVMContext> context;
    std::unique_ptr<Module> mod;
    std::unique_ptr<IRBuilder<>> builder;
};

