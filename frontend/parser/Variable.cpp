#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a variable declaration
// A variable declaration is composed of an Alloca and optionally, an assignment
bool Parser::buildVariableDec(AstBlock *block, Token idToken) {
    Token token = scanner->getNext();
    
    if (token.type != Colon) {
        syntax->addError(scanner->getLine(), "Error: Expected \':\' in declaration.");
        return false;
    }
    
    token = scanner->getNext();
    DataType dataType = DataType::Void;
    
    switch (token.type) {
        case Char: dataType = DataType::Char; break;
        case Int: dataType = DataType::Int32; break;
        
        default: {}
    }
    
    AstVarDec *vd = new AstVarDec(idToken.id_val, dataType);
    block->addStatement(vd);
    
    token = scanner->getNext();
    
    // We have an array
    if (token.type == LBracket) {
        vd->setDataType(DataType::Array);
        vd->setPtrType(dataType);
        
        if (!buildExpression(vd, RBracket)) return false;   
        
        token = scanner->getNext();
        if (token.type != SemiColon) {
            syntax->addError(scanner->getLine(), "Error: Expected \';\'.");
            return false;
        }
        
        // Create an assignment to a malloc call
        AstVarAssign *va = new AstVarAssign(idToken.id_val);
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
    
    // We're at the end of the declaration
    } else if (token.type == SemiColon) {
        return true;
        
    // Otherwise, we have a regular variable
    } else {
        scanner->rewind(token);

        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        block->addStatement(va);

        if (!buildExpression(va)) return false;
    }
    
    return true;
}

// Builds a variable assignment
bool Parser::buildVariableAssign(AstBlock *block, Token idToken) {
    AstVarAssign *va = new AstVarAssign(idToken.id_val);
    block->addStatement(va);
    
    if (!buildExpression(va)) return false;
    
    if (va->getExpressionCount() == 0) {
        syntax->addError(scanner->getLine(), "Invalid variable assignment.");
        return false;
    }
    
    return true;
}

// Builds an array assignment
bool Parser::buildArrayAssign(AstBlock *block, Token idToken) {
    AstArrayAssign *pa = new AstArrayAssign(idToken.id_val);
    block->addStatement(pa);
    
    if (!buildExpression(pa, RBracket)) return false;
    
    Token token = scanner->getNext();
    if (token.type != Assign) {
        syntax->addError(scanner->getLine(), "Expected \'=\' after array assignment.");
        return false;
    }
    
    if (!buildExpression(pa)) return false;

    return true;
}

