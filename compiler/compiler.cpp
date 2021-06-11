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
    // Create the global structure type
    std::vector<Type *> arrayTypes;
    arrayTypes.push_back(Type::getInt8PtrTy(*context));
    arrayTypes.push_back(Type::getInt32Ty(*context));
    arrayType = StructType::create(*context, arrayTypes);
    arrayType->setName("ArrayType");

    // Add declarations for built-in functions
    FunctionType *FT1 = FunctionType::get(Type::getInt8PtrTy(*context), Type::getInt32Ty(*context), false);
    Function::Create(FT1, Function::ExternalLinkage, "malloc", mod.get());

    // Build all other functions
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
            Type *type = translateType(vd->getDataType(), vd->getPtrType());
            bool isArray = false;
            
            if (vd->getDataType() == DataType::Ptr) {
                type = arrayType;
                isArray = true;
            }
            
            AllocaInst *var = builder->CreateAlloca(type);
            symtable[vd->getName()] = var;
            typeTable[vd->getName()] = vd->getDataType();
            
            // If we have an array, set the size of the structure
            if (isArray) {
                Value *size = compileValue(vd->getPtrSize());
                Value *sizePtr = builder->CreateStructGEP(var, 1);
                builder->CreateStore(size, sizePtr);
            }
        } break;
        
        // A variable assignment
        case AstType::VarAssign: {
            AstVarAssign *va = static_cast<AstVarAssign *>(stmt);
            AllocaInst *ptr = symtable[va->getName()];
            Value *val = compileValue(stmt->getExpressions().at(0));
            
            if (ptr->getType()->getElementType() == arrayType) {
                Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
                builder->CreateStore(val, arrayPtr);
            } else {
                builder->CreateStore(val, ptr);
            }
        } break;
        
        // A pointer (array) assignment
        case AstType::PtrAssign: {
            AstPtrAssign *pa = static_cast<AstPtrAssign *>(stmt);
            Value *ptr = symtable[pa->getName()];
            Value *index = compileValue(pa->getExpressions().at(0));
            Value *val = compileValue(pa->getExpressions().at(1));
            
            Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
            Value *ptrLd = builder->CreateLoad(arrayPtr);
            Value *ep = builder->CreateGEP(ptrLd, index);
            builder->CreateStore(val, ep);
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
            compileIfStatement(stmt);
        } break;
        
        // A while loop
        case AstType::While: {
            compileWhileStatement(stmt);
        } break;
        
        // An infinite loop
        case AstType::Loop: compileLoopStatement(stmt); break;
        
        // A for loop
        case AstType::For: {
            compileForStatement(stmt);
        } break;
        
        // A break statement
        case AstType::Break: {
            builder->CreateBr(breakStack.top());
        } break;
        
        // A continue statement
        case AstType::Continue: {
            builder->CreateBr(continueStack.top());
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
            //Type *type = translateType(typeTable[id->getValue()]);
            //return builder->CreateLoad(type, ptr);
            return builder->CreateLoad(ptr);
        } break;
        
        case AstType::Sizeof: {
            AstSizeof *sizeOf = static_cast<AstSizeof *>(expr);
            AstID *array = sizeOf->getValue();
            
            AllocaInst *ptr = symtable[array->getValue()];
            Value *sizePtr = builder->CreateStructGEP(ptr, 1);
            return builder->CreateLoad(sizePtr);
        } break;
        
        case AstType::ArrayAccess: {
            AstArrayAccess *acc = static_cast<AstArrayAccess *>(expr);
            AllocaInst *ptr = symtable[acc->getValue()];
            Value *index = compileValue(acc->getIndex());
            
            Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
            Value *ptrLd = builder->CreateLoad(arrayPtr);
            Value *ep = builder->CreateGEP(ptrLd, index);
            return builder->CreateLoad(ep);
        } break;
        
        case AstType::FuncCallExpr: {
            AstFuncCallExpr *fc = static_cast<AstFuncCallExpr *>(expr);
            std::vector<Value *> args;
            
            for (auto stmt : fc->getArguments()) {
                Value *val = compileValue(stmt);
                args.push_back(val);
            }
            
            Function *callee = mod->getFunction(fc->getName());
            if (!callee) std::cerr << "Invalid function call statement." << std::endl;
            return builder->CreateCall(callee, args);
        } break;
        
        case AstType::Add:
        case AstType::Sub: 
        case AstType::Mul:
        case AstType::Div:
        case AstType::EQ:
        case AstType::NEQ:
        case AstType::GT:
        case AstType::LT:
        case AstType::GTE:
        case AstType::LTE: {
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
                
            else if (expr->getType() == AstType::EQ)
                return builder->CreateICmpEQ(lval, rval);
            else if (expr->getType() == AstType::NEQ)
                return builder->CreateICmpNE(lval, rval);
            else if (expr->getType() == AstType::GT)
                return builder->CreateICmpSGT(lval, rval);
            else if (expr->getType() == AstType::LT)
                return builder->CreateICmpSLT(lval, rval);
            else if (expr->getType() == AstType::GTE)
                return builder->CreateICmpSGE(lval, rval);
            else if (expr->getType() == AstType::LTE)
                return builder->CreateICmpSLE(lval, rval);
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
                //case DataType::Int32: type = Type::getInt32PtrTy(*context); break;
                case DataType::Int32: type = arrayType; break;
                
                default: {}
            }
        } break;
        
        default: type = Type::getVoidTy(*context);
    }
    
    return type;
}

