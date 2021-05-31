#include "llvm/IR/BasicBlock.h"

using namespace llvm;

#include <iostream>

#include <compiler.hpp>

Compiler::Compiler(AstTree *tree) {
    this->tree = tree;

    context = std::make_unique<LLVMContext>();
    mod = std::make_unique<Module>("mod1", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
}

void Compiler::compile() {
    for (auto global : tree->getGlobalStatements()) {
        switch (global->getType()) {
            case AstType::Func: {
                AstFunction *astFunc = static_cast<AstFunction *>(global);

                FunctionType *FT = FunctionType::get(Type::getVoidTy(*context), Type::getVoidTy(*context), false);
                Function *func = Function::Create(FT, Function::ExternalLinkage, astFunc->getName(), mod.get());

                BasicBlock *mainBlock = BasicBlock::Create(*context, "entry", func);
                builder->SetInsertPoint(mainBlock);

                for (auto stmt : astFunc->getCode()) {
                    compileStatement(stmt);
                }
            } break;

            default: {}
        }
    }
}

void Compiler::debug() {
    mod->print(errs(), nullptr);
}

// Compiles an individual statement
void Compiler::compileStatement(AstStatement *stmt) {
    switch (stmt->getType()) {
        // A variable declaration (alloca) statement
        case AstType::VarDec: {
            AstVarDec *vd = static_cast<AstVarDec *>(stmt);
            Type *type;
            
            switch (vd->getDataType()) {
                case DataType::Int32: type = Type::getInt32Ty(*context); break;
                default: type = Type::getVoidTy(*context);
            }
            
            Value *var = builder->CreateAlloca(type);
            symtable[vd->getName()] = var;
        } break;
        
        // A variable assignment
        case AstType::VarAssign: {
            AstVarAssign *va = static_cast<AstVarAssign *>(stmt);
            Value *ptr = symtable[va->getName()];
            
            if (stmt->getExpressionCount() == 1) {
                Value *val = compileValue(stmt->getExpressions().at(0));
                builder->CreateStore(val, ptr);
            } else {
            
            }
        } break;
        
        // A return statement
        case AstType::Return: {
            if (stmt->getExpressionCount() == 0) {
                builder->CreateRetVoid();
            } else if (stmt->getExpressionCount() == 1) {
                Value *val = compileValue(stmt->getExpressions().at(0));
                builder->CreateRet(val);
            } else {
                builder->CreateRetVoid();
            }
        } break;
        
        default: {}
    }
}

// Converts an AST value to an LLVM value
Value *Compiler::compileValue(AstExpression *expr) {
    switch (expr->getType()) {
        case AstType::IntL: {
            AstInt *ival = static_cast<AstInt *>(expr);
            return builder->getInt32(ival->getValue());
        }
    }
    
    return nullptr;
}

