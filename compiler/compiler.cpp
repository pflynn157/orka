#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;
using namespace llvm::sys;

#include <iostream>

#include <compiler.hpp>

Compiler::Compiler(AstTree *tree, CFlags cflags) {
    this->tree = tree;
    this->cflags = cflags;

    context = std::make_unique<LLVMContext>();
    mod = std::make_unique<Module>(cflags.name, *context);
    builder = std::make_unique<IRBuilder<>>(*context);
}

void Compiler::compile() {
    for (auto global : tree->getGlobalStatements()) {
        switch (global->getType()) {
            case AstType::Func: {
                AstFunction *astFunc = static_cast<AstFunction *>(global);

                std::vector<Var> astVarArgs = astFunc->getArguments();
                FunctionType *FT;
                
                if (astVarArgs.size() == 0) {
                    FT = FunctionType::get(Type::getVoidTy(*context), false);
                } else {
                    std::vector<Type *> args;
                    for (auto var : astVarArgs) {
                        Type *type = translateType(var.type, var.subType);
                        args.push_back(type);
                    }
                    
                    FT = FunctionType::get(Type::getVoidTy(*context), args, false);
                }
                
                Function *func = Function::Create(FT, Function::ExternalLinkage, astFunc->getName(), mod.get());
                currentFunc = func;

                BasicBlock *mainBlock = BasicBlock::Create(*context, "entry", func);
                builder->SetInsertPoint(mainBlock);
                
                // Load and store any arguments
                if (astVarArgs.size() > 0) {
                    for (int i = 0; i<astVarArgs.size(); i++) {
                        Var var = astVarArgs.at(i);
                        
                        // Build the alloca for the local var
                        Type *type = translateType(var.type, var.subType);
                        AllocaInst *alloca = builder->CreateAlloca(type);
                        symtable[var.name] = alloca;
                        typeTable[var.name] = var.type;
                        
                        // Store the variable
                        Value *param = func->getArg(i);
                        builder->CreateStore(param, alloca);
                    }
                }

                for (auto stmt : astFunc->getBlock()->getBlock()) {
                    compileStatement(stmt);
                }
            } break;
            
            case AstType::ExternFunc: {
                AstExternFunction *astFunc = static_cast<AstExternFunction *>(global);
                
                std::vector<Var> astVarArgs = astFunc->getArguments();
                FunctionType *FT;
                
                if (astVarArgs.size() == 0) {
                    FT = FunctionType::get(Type::getVoidTy(*context), false);
                } else {
                    std::vector<Type *> args;
                    for (auto var : astVarArgs) {
                        Type *type = translateType(var.type, var.subType);
                        args.push_back(type);
                    }
                    
                    FT = FunctionType::get(Type::getVoidTy(*context), args, false);
                }
                
                Function::Create(FT, Function::ExternalLinkage, astFunc->getName(), mod.get());
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
        
        // TODO: We should not do error handeling in the compiler. Check for invalid functions in the AST level
        // Function call statements
        case AstType::FuncCallStmt: {
            AstFuncCallStmt *fc = static_cast<AstFuncCallStmt *>(stmt);
            std::vector<Value *> args;
            
            for (auto stmt : stmt->getExpressions()) {
                Value *val = compileValue(stmt);
                args.push_back(val);
            }
            
            Function *callee = mod->getFunction(fc->getName());
            if (!callee) std::cerr << "Invalid function call statement." << std::endl;
            builder->CreateCall(callee, args);
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
        
        // An IF statement
        case AstType::If: {
            BasicBlock *trueBlock = BasicBlock::Create(*context, "true" + std::to_string(blockCount), currentFunc);
            BasicBlock *falseBlock = BasicBlock::Create(*context, "false" + std::to_string(blockCount), currentFunc);
            BasicBlock *endBlock = BasicBlock::Create(*context, "end" + std::to_string(blockCount), currentFunc);
            ++blockCount;
            
            Value *cond = compileValue(stmt->getExpressions().at(0));
            builder->CreateCondBr(cond, trueBlock, falseBlock);
            
            // Align the blocks
            BasicBlock *current = builder->GetInsertBlock();
            trueBlock->moveAfter(current);
            falseBlock->moveAfter(trueBlock);
            endBlock->moveAfter(falseBlock);
            
            builder->SetInsertPoint(trueBlock);
            //Instruction *br = builder->CreateBr(endBlock);
            //builder->SetInsertPoint(br);
            
            endBlockStack.push(endBlock);
            blockStack.push(falseBlock);
        } break;
        
        // An ELSE statement
        case AstType::Else: {
            /*if (blockStack.size() > 0) {
                BasicBlock *endBlock = endBlockStack.top();
                
                BasicBlock *block = blockStack.top();
                blockStack.pop();
                
                builder->CreateBr(endBlock);
                block->insertInto(currentFunc);
                builder->SetInsertPoint(block);
            }*/
        } break;
        
        // A while loop
        case AstType::While: {
            BasicBlock *loopBlock = BasicBlock::Create(*context, "loop_body" + std::to_string(blockCount), currentFunc);
            BasicBlock *loopCmp = BasicBlock::Create(*context, "loop_cmp" + std::to_string(blockCount), currentFunc);
            BasicBlock *loopEnd = BasicBlock::Create(*context, "loop_end" + std::to_string(blockCount), currentFunc);
            ++blockCount;
            
            /*BasicBlock *current = builder->GetInsertBlock();
            loopBlock->moveAfter(current);
            loopCmp->moveAfter(loopBlock);
            loopEnd->moveAfter(loopCmp);*/
            
            builder->CreateBr(loopCmp);
            builder->SetInsertPoint(loopCmp);
            Value *cond = compileValue(stmt->getExpressions().at(0));
            builder->CreateCondBr(cond, loopBlock, loopEnd);
            
            builder->SetInsertPoint(loopBlock);
            Instruction *br = builder->CreateBr(loopCmp);
            
            endBlockStack.push(loopEnd);
        } break;
        
        // The end of a block
        case AstType::End: {
            if (blockStack.size() > 0) {
                BasicBlock *block = blockStack.top();
                blockStack.pop();
                
                builder->SetInsertPoint(block);
                builder->CreateBr(endBlockStack.top());
            }
        
            if (endBlockStack.size() > 0) {
                BasicBlock *end = endBlockStack.top();
                endBlockStack.pop();
                
                builder->SetInsertPoint(end);
                
                BasicBlock *block = BasicBlock::Create(*context, "", currentFunc);
                builder->CreateBr(block);
                
                block->moveAfter(builder->GetInsertBlock());
                builder->SetInsertPoint(block);
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
        } break;
        
        case AstType::StringL: {
            AstString *str = static_cast<AstString *>(expr);
            return builder->CreateGlobalStringPtr(str->getValue());
        } break;
        
        case AstType::ID: {
            AstID *id = static_cast<AstID *>(expr);
            AllocaInst *ptr = symtable[id->getValue()];
            Type *type = translateType(typeTable[id->getValue()]);
            return builder->CreateLoad(type, ptr);
        } break;
        
        case AstType::Add:
        case AstType::Sub: 
        case AstType::Mul:
        case AstType::Div:
        case AstType::GT:
        case AstType::LT: {
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
            else if (expr->getType() == AstType::GT)
                return builder->CreateICmpSGT(lval, rval);
            else if (expr->getType() == AstType::LT)
                return builder->CreateICmpSLT(lval, rval);
        } break;
        
        default: {}
    }
    
    return nullptr;
}

Type *Compiler::translateType(DataType dataType, DataType subType) {
    Type *type;
            
    switch (dataType) {
        case DataType::Int32: type = Type::getInt32Ty(*context); break;
        
        case DataType::Ptr: {
            switch (subType) {
                case DataType::Char: type = Type::getInt8PtrTy(*context); break;
                
                default: {}
            }
        } break;
        
        default: type = Type::getVoidTy(*context);
    }
    
    return type;
}

