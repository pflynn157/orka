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
    do {
        token = scanner->getNext();
    } while (token.type != Eof && token.type != Begin);
    
    // Build the body
    do {
        token = scanner->getNext();
        
        switch (token.type) {
            case Return: buildReturn(func); break;
            
            case End: 
            case Nl: break;
            default: token.print();
        }
    } while (token.type != Eof && token.type != End);
}

void Frontend::buildReturn(AstFunction *func) {
    AstReturnStmt *stmt = new AstReturnStmt;
    func->addStatement(stmt);
    
    buildExpression(stmt);
}

void Frontend::buildExpression(AstStatement *stmt) {
    Token token;
    do {
        token = scanner->getNext();
        
        switch (token.type) {
            case Int32: {
                AstInt *i32 = new AstInt(token.i32_val);
                stmt->addExpression(i32);
            } break;
        }
    } while (token.type != Eof && token.type != SemiColon);
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

