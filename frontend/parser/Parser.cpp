#include <iostream>

#include <parser/Parser.hpp>

Parser::Parser(std::string input) {
    this->input = input;
    scanner = new Scanner(input);
    
    tree = new AstTree(input);
    syntax = new ErrorManager;
}

Parser::~Parser() {
    delete scanner;
    delete syntax;
}

bool Parser::parse() {
    Token token;
    do {
        token = scanner->getNext();
        bool code = true;
        
        switch (token.type) {
            case Func: {
                code = buildFunction();
            } break;
            
            case Extern: {
                code = buildExternFunction();
            } break;
            
            default: {}
        }
        
        if (!code) break;
    } while (token.type != Eof);
    
    // Check for errors, and print if so
    if (syntax->errorsPresent()) {
        syntax->printErrors();
        return false;
    }
    
    syntax->printWarnings();
    return true;
}

// Builds a conditional statement
bool Parser::buildConditional(AstBlock *block) {
    AstIfStmt *cond = new AstIfStmt;
    if (!buildExpression(cond, Nl)) return false;
    block->addStatement(cond);

    ++layer;
    return true;
}

// Builds a while statement
bool Parser::buildWhile(AstBlock *block) {
    AstWhileStmt *loop = new AstWhileStmt;
    if (!buildExpression(loop, Nl)) return false;
    block->addStatement(loop);
    
    ++layer;
    return true;
}

// Builds an expression
bool Parser::buildExpression(AstStatement *stmt, TokenType stopToken, TokenType separateToken) {
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
            
            case GT: {
                AstGTOp *gt = new AstGTOp;
                opStack.push(gt);
            } break;
            
            case LT: opStack.push(new AstLTOp); break;
            
            default: {}
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
        return true;
    }
    
    AstExpression *expr = output.top();
    stmt->addExpression(expr);
    
    return true;
}

// The debug function for the scanner
void Parser::debugScanner() {
    std::cout << "Debugging scanner..." << std::endl;
    
    Token t;
    do {
        t = scanner->getNext();
        t.print();
    } while (t.type != Eof);
}

