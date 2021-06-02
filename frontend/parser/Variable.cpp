#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a variable declaration
// A variable declaration is composed of an Alloca and optionally, an assignment
void Parser::buildVariableDec(AstFunction *func, Token idToken) {
    Token token = scanner->getNext();
    
    if (token.type != Colon) {
        std::cerr << "Error: Expected \':\' in declaration." << std::endl;
        token.print();
        return;
    }
    
    token = scanner->getNext();
    DataType dataType = DataType::Void;
    
    switch (token.type) {
        case Int: dataType = DataType::Int32; break;
    }
    
    AstVarDec *vd = new AstVarDec(idToken.id_val, dataType);
    func->addStatement(vd);
    
    token = scanner->getNext();
    if (token.type != SemiColon) {
        scanner->rewind(token);
        
        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        func->addStatement(va);
        
        buildExpression(va);
    }
}

// Builds a variable assignment
void Parser::buildVariableAssign(AstFunction *func, Token idToken) {
    AstVarAssign *va = new AstVarAssign(idToken.id_val);
    func->addStatement(va);
    
    buildExpression(va);
    
    if (va->getExpressionCount() == 0) {
        std::cerr << "Error: Invalid variable assignment." << std::endl;
    }
}

