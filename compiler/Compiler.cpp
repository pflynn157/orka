#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;
using namespace llvm::sys;

#include <iostream>

#include <Compiler.hpp>

Compiler::Compiler(AstTree *tree, CFlags cflags) {
    this->tree = tree;
    this->cflags = cflags;

    context = std::make_unique<LLVMContext>();
    mod = std::make_unique<Module>(cflags.name, *context);
    builder = std::make_unique<IRBuilder<>>(*context);
    
    // Create the global structure type
    std::vector<Type *> arrayTypes;
    arrayTypes.push_back(Type::getInt32PtrTy(*context));
    arrayTypes.push_back(Type::getInt32Ty(*context));
    i32ArrayType = StructType::create(*context, arrayTypes);
    i32ArrayType->setName("IntArrayType");
    
    arrayTypes.clear();
    arrayTypes.push_back(Type::getInt64PtrTy(*context));
    arrayTypes.push_back(Type::getInt32Ty(*context));
    i64ArrayType = StructType::create(*context, arrayTypes);
    i64ArrayType->setName("Int64ArrayType");
    
    arrayTypes.clear();
    arrayTypes.push_back(Type::getInt8PtrTy(*context));
    arrayTypes.push_back(Type::getInt32Ty(*context));
    i8ArrayType = StructType::create(*context, arrayTypes);
    i8ArrayType->setName("CharArrayType");

    // Add declarations for built-in functions
    FunctionType *FT1 = FunctionType::get(Type::getInt8PtrTy(*context), Type::getInt32Ty(*context), false);
    Function::Create(FT1, Function::ExternalLinkage, "malloc", mod.get());
}

void Compiler::compile() {
    // Build all other functions
    for (auto global : tree->getGlobalStatements()) {
        switch (global->getType()) {
            case AstType::Func: {
                symtable.clear();
                typeTable.clear();
                ptrTable.clear();
                
                compileFunction(global);
            } break;
            
            case AstType::ExternFunc: {
                compileExternFunction(global);
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
            
            AllocaInst *var = builder->CreateAlloca(type);
            symtable[vd->getName()] = var;
            typeTable[vd->getName()] = vd->getDataType();
            ptrTable[vd->getName()] = vd->getPtrType();
            
            // If we have an array, set the size of the structure
            if (vd->getDataType() == DataType::Array) {
                Value *size = compileValue(vd->getPtrSize());
                Value *sizePtr = builder->CreateStructGEP(var, 1);
                builder->CreateStore(size, sizePtr);
            }
        } break;
        
        // A variable assignment
        case AstType::VarAssign: {
            AstVarAssign *va = static_cast<AstVarAssign *>(stmt);
            AllocaInst *ptr = symtable[va->getName()];
            DataType ptrType = typeTable[va->getName()];
            Value *val = compileValue(stmt->getExpressions().at(0), ptrType);
            
            if (ptrType == DataType::Array) {
                Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
                builder->CreateStore(val, arrayPtr);
            } else {
                builder->CreateStore(val, ptr);
            }
        } break;
        
        // An array assignment
        case AstType::ArrayAssign: {
            AstArrayAssign *pa = static_cast<AstArrayAssign *>(stmt);
            Value *ptr = symtable[pa->getName()];
            DataType ptrType = typeTable[pa->getName()];
            DataType subType = ptrTable[pa->getName()];
            
            Value *index = compileValue(pa->getExpressions().at(0));
            Value *val = compileValue(pa->getExpressions().at(1), subType);
            
            if (ptrType == DataType::String) {
                Value *arrayPtr = builder->CreateLoad(ptr);
                Value *ep = builder->CreateGEP(arrayPtr, index);
                builder->CreateStore(val, ep);
            } else {
                Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
                Value *ptrLd = builder->CreateLoad(arrayPtr);
                Value *ep = builder->CreateGEP(ptrLd, index);
                builder->CreateStore(val, ep);
            }
        } break;
        
        // Function call statements
        case AstType::FuncCallStmt: {
            compileFuncCallStatement(stmt);
        } break;
        
        // A return statement
        case AstType::Return: {
            compileReturnStatement(stmt);
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
Value *Compiler::compileValue(AstExpression *expr, DataType dataType) {
    switch (expr->getType()) {
        case AstType::BoolL: {
            AstBool *b = static_cast<AstBool *>(expr);
            return builder->getInt1(b->getValue());
        } break;
    
        case AstType::ByteL: {
            AstByte *i8 = static_cast<AstByte *>(expr);
            return builder->getInt8(i8->getValue());
        } break;
        
        case AstType::WordL: {
            AstWord *i16 = static_cast<AstWord *>(expr);
            return builder->getInt16(i16->getValue());
        } break;
        
        case AstType::IntL: {
            AstInt *ival = static_cast<AstInt *>(expr);
            return builder->getInt32(ival->getValue());
        } break;
        
        case AstType::QWordL: {
            AstQWord *i64 = static_cast<AstQWord *>(expr);
            return builder->getInt64(i64->getValue());
        } break;
        
        case AstType::CharL: {
            AstChar *cval = static_cast<AstChar *>(expr);
            return builder->getInt8(cval->getValue());
        } break;
        
        case AstType::StringL: {
            AstString *str = static_cast<AstString *>(expr);
            return builder->CreateGlobalStringPtr(str->getValue());
        } break;
        
        case AstType::ID: {
            AstID *id = static_cast<AstID *>(expr);
            AllocaInst *ptr = symtable[id->getValue()];
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
            DataType ptrType = typeTable[acc->getValue()];
            Value *index = compileValue(acc->getIndex());
            
            if (ptrType == DataType::String) {
                Value *arrayPtr = builder->CreateLoad(ptr);
                Value *ep = builder->CreateGEP(arrayPtr, index);
                return builder->CreateLoad(ep);
            } else {
                Value *arrayPtr = builder->CreateStructGEP(ptr, 0);
                Value *ptrLd = builder->CreateLoad(arrayPtr);
                Value *ep = builder->CreateGEP(ptrLd, index);
                return builder->CreateLoad(ep);
            }
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
            Value *lval = compileValue(op->getLVal(), dataType);
            Value *rval = compileValue(op->getRVal(), dataType);
            
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
        case DataType::Bool: type = Type::getInt1Ty(*context); break;
    
        case DataType::Char:
        case DataType::Byte:
        case DataType::UByte: type = Type::getInt8Ty(*context); break;
        
        case DataType::Short:
        case DataType::UShort: type = Type::getInt16Ty(*context); break;
        
        case DataType::Int32:
        case DataType::UInt32: type = Type::getInt32Ty(*context); break;
        
        case DataType::Int64:
        case DataType::UInt64: type = Type::getInt64Ty(*context); break;
        
        case DataType::String: type = Type::getInt8PtrTy(*context); break;
        
        case DataType::Array: {
            switch (subType) {
                case DataType::Char: type = i8ArrayType; break;
                
                case DataType::UInt32:
                case DataType::Int32: type = i32ArrayType; break;
                
                case DataType::Int64:
                case DataType::UInt64: type = i64ArrayType; break;
                
                default: {}
            }
        } break;
        
        case DataType::Ptr: {
            switch (subType) {
                case DataType::Char: type = Type::getInt8PtrTy(*context); break;
                case DataType::Int32: type = Type::getInt32PtrTy(*context); break;
                
                default: {}
            }
        } break;
        
        default: type = Type::getVoidTy(*context);
    }
    
    return type;
}

