#include <iostream>

#include <frontend.hpp>

Frontend::Frontend(std::string input) {
    this->input = input;
    scanner = new Scanner(input);
    
    tree = new AstTree(input);
}

Frontend::~Frontend() {
    delete scanner;
}

void Frontend::parse() {
    Token token;
    do {
        token = scanner->getNext();
        
        switch (token.type) {
            case Func: {
                buildFunction();
            } break;
        }
    } while (token.type != Eof);
}

void Frontend::buildFunction() {
    Token token = scanner->getNext();
    if (token.type != Id) {
        // TODO: Error
        std::cout << "Error: Expected ID" << std::endl;
        return;
    }

    AstFunction *func = new AstFunction(token.id_val);
    tree->addGlobalStatement(func);
    
    // Build the variable body
    token = scanner->getNext();
    while (token.type != Eof && token.type != Begin) {
        switch (token.type) {
            case Id: buildVariableDec(func, token); break;
            
            default: token.print();
        }
        
        token = scanner->getNext();
    }
    
    // Build the body
    token = scanner->getNext();
    while (token.type != Eof && token.type != End) {
        switch (token.type) {
            case Id: {
                Token idToken = token;
                token = scanner->getNext();
                
                if (token.type == Assign) {
                    buildVariableAssign(func, idToken);
                } else {
                    std::cerr << "Invalid ID" << std::endl;
                }
            } break;
            
            case Return: buildReturn(func); break;
            
            case End: 
            case Nl: break;
            default: token.print();
        }
        
        token = scanner->getNext();
    }
}

// Builds a variable declaration
// A variable declaration is composed of an Alloca and optionally, an assignment
void Frontend::buildVariableDec(AstFunction *func, Token idToken) {
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
    
    buildExpression(vd);
}

// Builds a variable assignment
void Frontend::buildVariableAssign(AstFunction *func, Token idToken) {
    AstVarAssign *va = new AstVarAssign(idToken.id_val);
    func->addStatement(va);
    
    buildExpression(va);
    
    if (va->getExpressionCount() == 0) {
        std::cerr << "Error: Invalid variable assignment." << std::endl;
    }
}

void Frontend::buildReturn(AstFunction *func) {
    AstReturnStmt *stmt = new AstReturnStmt;
    func->addStatement(stmt);
    
    buildExpression(stmt);
}

void Frontend::buildExpression(AstStatement *stmt) {
    Token token = scanner->getNext();
    while (token.type != Eof && token.type != SemiColon) {
        switch (token.type) {
            case Int32: {
                AstInt *i32 = new AstInt(token.i32_val);
                stmt->addExpression(i32);
            } break;
        }
        
        token = scanner->getNext();
    }
}

// The debug function for the scanner
void Frontend::debugScanner() {
    std::cout << "Debugging scanner..." << std::endl;
    
    Token t;
    do {
        t = scanner->getNext();
        t.print();
    } while (t.type != Eof);
}

