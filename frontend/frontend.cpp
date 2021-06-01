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
            
            case Nl: break;
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
    
    token = scanner->getNext();
    if (token.type != SemiColon) {
        scanner->rewind(token);
        
        AstVarAssign *va = new AstVarAssign(idToken.id_val);
        func->addStatement(va);
        
        buildExpression(va);
    }
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

// Builds an expression
void Frontend::buildExpression(AstStatement *stmt) {
    std::stack<AstExpression *> output;
    std::stack<AstExpression *> opStack;

    Token token = scanner->getNext();
    while (token.type != Eof && token.type != SemiColon) {
        switch (token.type) {
            case Int32: {
                AstInt *i32 = new AstInt(token.i32_val);
                output.push(i32);
            } break;
            
            case Id: {
                AstID *id = new AstID(token.id_val);
                output.push(id);
            } break;
            
            case Plus: {
                AstAddOp *add = new AstAddOp;
                opStack.push(add);
            } break;
        }
        
        token = scanner->getNext();
    }
    
    // Build the expression
    while (opStack.size() > 0) {
        AstExpression *rval = output.top();
        output.pop();
        
        AstExpression *lval = output.top();
        output.pop();
        
        AstBinaryOp *op = static_cast<AstBinaryOp *>(opStack.top());
        opStack.pop();
        
        op->setLVal(lval);
        op->setRVal(rval);
        output.push(op);
    }
    
    // Add the expressions back
    if (output.size() == 0) {
        return;
    }
    
    AstExpression *expr = output.top();
    stmt->addExpression(expr);
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

