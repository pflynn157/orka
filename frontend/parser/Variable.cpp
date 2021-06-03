#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a variable declaration
// A variable declaration is composed of an Alloca and optionally, an assignment
bool Parser::buildVariableDec(AstFunction *func, Token idToken) {
    Token token = scanner->getNext();
    
    if (token.type != Colon) {
        syntax->addError(scanner->getLine(), "Error: Expected \':\' in declaration.");
        return false;
    }
    
    token = scanner->getNext();
    DataType dataType = DataType::Void;
    
    switch (token.type) {
        case Int: dataType = DataType::Int32; break;
        
        default: {}
    }
    
    AstVarDec *vd = new AstVarDec(idToken.id_val, dataType);
    func->addStatement(vd);
    
    token = scanner->getNext();
    if (token.type != SemiColon) {
        scanner->rewind(token);
        
        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        func->addStatement(va);
        
        if (!buildExpression(va)) return false;
    }
    
    return true;
}

// Builds a variable assignment
bool Parser::buildVariableAssign(AstFunction *func, Token idToken) {
    AstVarAssign *va = new AstVarAssign(idToken.id_val);
    func->addStatement(va);
    
    if (!buildExpression(va)) return false;
    
    if (va->getExpressionCount() == 0) {
        syntax->addError(scanner->getLine(), "Invalid variable assignment.");
        return false;
    }
    
    return true;
}

