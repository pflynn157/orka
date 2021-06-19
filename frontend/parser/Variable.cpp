#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a variable declaration
// A variable declaration is composed of an Alloca and optionally, an assignment
bool Parser::buildVariableDec(AstBlock *block) {
    Token idToken = scanner->getNext();
    
    if (idToken.type != Id) {
        syntax->addError(scanner->getLine(), "Expected variable name.");
        return false;
    }
    
    Token token = scanner->getNext();
    
    if (token.type != Colon) {
        syntax->addError(scanner->getLine(), "Expected \':\' in declaration.");
        return false;
    }
    
    token = scanner->getNext();
    DataType dataType = DataType::Void;
    bool isString = false;
    
    switch (token.type) {
        case Char: dataType = DataType::Char; break;
        case Byte: dataType = DataType::Byte; break;
        case UByte: dataType = DataType::UByte; break;
        case Short: dataType = DataType::Short; break;
        case UShort: dataType = DataType::UShort; break;
        case Int: dataType = DataType::Int32; break;
        case UInt: dataType = DataType::UInt32; break;
        case Int64: dataType = DataType::Int64; break;
        case UInt64: dataType = DataType::UInt64; break;
        case Str: dataType = DataType::String; break;
        
        default: {}
    }
    
    AstVarDec *vd = new AstVarDec(idToken.id_val, dataType);
    block->addStatement(vd);
    
    token = scanner->getNext();
    
    // We have an array
    if (token.type == LBracket) {
        vd->setDataType(DataType::Array);
        vd->setPtrType(dataType);
        
        if (!buildExpression(vd, DataType::Int32, RBracket)) return false;   
        
        token = scanner->getNext();
        if (token.type != SemiColon) {
            syntax->addError(scanner->getLine(), "Error: Expected \';\'.");
            return false;
        }
        
        // Create an assignment to a malloc call
        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        va->setDataType(DataType::Array);
        va->setPtrType(dataType);
        block->addStatement(va);
        
        AstFuncCallExpr *callMalloc = new AstFuncCallExpr("malloc");
        callMalloc->setArguments(vd->getExpressions());
        va->addExpression(callMalloc);
        
        // In order to get a proper malloc, we need to multiply the argument by
        // the size of the type. Get the arguments, and do that
        AstExpression *arg = callMalloc->getArguments().at(0);
        callMalloc->clearArguments();
        
        AstInt *size;
        if (dataType == DataType::Int32) size = new AstInt(4);
        else size = new AstInt(1);
        
        AstMulOp *op = new AstMulOp;
        op->setLVal(size);
        op->setRVal(arg);
        callMalloc->addArgument(op);
        
        // Finally, set the size of the declaration
        vd->setPtrSize(arg);
        
        typeMap[idToken.id_val] = std::pair<DataType, DataType>(DataType::Array, dataType);
    
    // We're at the end of the declaration
    } else if (token.type == SemiColon) {
        return true;
        
    // Otherwise, we have a regular variable
    } else {
        auto typePair = std::pair<DataType, DataType>(dataType, DataType::Void);
        typeMap[idToken.id_val] = typePair;
        scanner->rewind(token);

        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        va->setDataType(dataType);
        block->addStatement(va);

        if (!buildExpression(va, dataType)) return false;
    }
    
    return true;
}

// Builds a variable assignment
bool Parser::buildVariableAssign(AstBlock *block, Token idToken) {
    DataType dataType = typeMap[idToken.id_val].first;
    AstVarAssign *va = new AstVarAssign(idToken.id_val);
    va->setDataType(dataType);
    block->addStatement(va);
    
    if (!buildExpression(va, dataType)) return false;
    
    if (va->getExpressionCount() == 0) {
        syntax->addError(scanner->getLine(), "Invalid variable assignment.");
        return false;
    }
    
    return true;
}

// Builds an array assignment
bool Parser::buildArrayAssign(AstBlock *block, Token idToken) {
    DataType dataType = typeMap[idToken.id_val].second;
    AstArrayAssign *pa = new AstArrayAssign(idToken.id_val);
    pa->setDataType(typeMap[idToken.id_val].first);
    pa->setPtrType(dataType);
    block->addStatement(pa);
    
    if (!buildExpression(pa, DataType::Int32, RBracket)) return false;
    
    Token token = scanner->getNext();
    if (token.type != Assign) {
        syntax->addError(scanner->getLine(), "Expected \'=\' after array assignment.");
        return false;
    }
    
    if (!buildExpression(pa, dataType)) return false;

    return true;
}

