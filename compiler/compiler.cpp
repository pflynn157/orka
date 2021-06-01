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
            Type *type = translateType(vd->getDataType());
            
            AllocaInst *var = builder->CreateAlloca(type);
            symtable[vd->getName()] = var;
            typeTable[vd->getName()] = vd->getDataType();
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
        
        case AstType::ID: {
            AstID *id = static_cast<AstID *>(expr);
            AllocaInst *ptr = symtable[id->getValue()];
            Type *type = translateType(typeTable[id->getValue()]);
            return builder->CreateLoad(type, ptr);
        } break;
        
        case AstType::Add:
        case AstType::Sub: 
        case AstType::Mul:
        case AstType::Div: {
            AstBinaryOp *op = static_cast<AstBinaryOp *>(expr);
            Value *lval = compileValue(op->getLVal());
            Value *rval = compileValue(op->getRVal());
            
            if (expr->getType() == AstType::Add)
                return builder->CreateAdd(lval, rval);
            else if (expr->getType() == AstType::Sub)
                return builder->CreateSub(lval, rval);
            else if (expr->getType() == AstType::Mul)
                return builder->CreateMul(lval, rval);
            else if (expr->getType() == AstType::Div)
                return builder->CreateSDiv(lval, rval);
        } break;
    }
    
    return nullptr;
}

Type *Compiler::translateType(DataType dataType) {
    Type *type;
            
    switch (dataType) {
        case DataType::Int32: type = Type::getInt32Ty(*context); break;
        default: type = Type::getVoidTy(*context);
    }
    
    return type;
}

