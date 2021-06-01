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
            
            case Extern: {
                buildExternFunction();
            } break;
        }
    } while (token.type != Eof);
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

// Builds a function call
void Frontend::buildFunctionCallStmt(AstFunction *func, Token idToken) {
    AstFuncCallStmt *fc = new AstFuncCallStmt(idToken.id_val);
    func->addStatement(fc);
    
    buildExpression(fc, RParen, Comma);
    
    Token token = scanner->getNext();
    if (token.type != SemiColon) {
        std::cerr << "Error: Expected \';\'." << std::endl;
        return;
    }
}

void Frontend::buildReturn(AstFunction *func) {
    AstReturnStmt *stmt = new AstReturnStmt;
    func->addStatement(stmt);
    
    buildExpression(stmt);
}

// Builds an expression
void Frontend::buildExpression(AstStatement *stmt, TokenType stopToken, TokenType separateToken) {
    std::stack<AstExpression *> output;
    std::stack<AstExpression *> opStack;

    Token token = scanner->getNext();
    while (token.type != Eof && token.type != stopToken) {
        if (token.type == separateToken && output.size() > 0) {
            AstExpression *expr = output.top();
            output.pop();
            
            stmt->addExpression(expr);
            continue;
        }
    
        switch (token.type) {
            case Int32: {
                AstInt *i32 = new AstInt(token.i32_val);
                output.push(i32);
            } break;
            
            case String: {
                AstString *str = new AstString(token.id_val);
                output.push(str);
            } break;
            
            case Id: {
                AstID *id = new AstID(token.id_val);
                output.push(id);
            } break;
            
            case Plus: {
                AstAddOp *add = new AstAddOp;
                opStack.push(add);
            } break;
            
            case Minus: {
                AstSubOp *sub = new AstSubOp;
                opStack.push(sub);
            } break;
            
            case Mul: {
                AstMulOp *mul = new AstMulOp;
                opStack.push(mul);
            } break;
            
            case Div: {
                AstDivOp *div = new AstDivOp;
                opStack.push(div);
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

