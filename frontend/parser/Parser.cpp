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
            
            case Eof:
            case Nl: break;
            
            default: {
                syntax->addError(scanner->getLine(), "Invalid token in global scope.");
                token.print();
                code = false;
            }
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

// Builds a statement block
bool Parser::buildBlock(AstBlock *block, int stopLayer, AstIfStmt *parentBlock, bool inElif) {
    Token token = scanner->getNext();
    while (token.type != Eof) {
        bool code = true;
        bool end = false;
        
        switch (token.type) {
            case Id: {
                Token idToken = token;
                token = scanner->getNext();
                
                if (token.type == Assign) {
                    code = buildVariableAssign(block, idToken);
                } else if (token.type == LBracket) {
                    code = buildArrayAssign(block, idToken);
                } else if (token.type == LParen) {
                    code = buildFunctionCallStmt(block, idToken);
                } else {
                    syntax->addError(scanner->getLine(), "Invalid use of identifier.");
                    token.print();
                    return false;
                }
            } break;
            
            case Return: code = buildReturn(block); break;
            
            // Handle conditionals
            // Yes, ELIF and ElSE are similar, but if you look closely, there is a subtle
            // difference (one very much needed)
            case If: code = buildConditional(block); break;
            case Elif: {
                if (inElif) {
                    scanner->rewind(token);
                    end = true;
                } else {
                    code = buildElif(parentBlock);
                }
            } break;
            case Else: {
                if (inElif) {
                    scanner->rewind(token);
                    end = true;
                } else {
                    code = buildElse(parentBlock);
                    end = true;
                }
            } break;
            
            // Handle loops
            case While: code = buildWhile(block); break;
            case Loop: code = buildLoop(block); break;
            case For: code = buildFor(block); break;
            
            case Break: code = buildLoopCtrl(block, true); break;
            case Continue: code = buildLoopCtrl(block, false); break;
            
            // Handle the END keyword
            // This is kind of tricky in conditionals
            case End: {
                if (inElif) {
                    scanner->rewind(token);
                    end = true;
                    break;
                }
                if (layer == stopLayer) {
                    end = true;
                }
                if (layer > 0) --layer;
            } break;
            
            case Nl: break;
            
            default: {
                syntax->addError(scanner->getLine(), "Invalid token in expression.");
                token.print();
                return false;
            }
        }
        
        if (end) break;
        if (!code) return false;
        token = scanner->getNext();
    }
    
    return true;
}

// Builds an expression
bool Parser::buildExpression(AstStatement *stmt, TokenType stopToken, TokenType separateToken, AstExpression **dest) {
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
                std::string name = token.id_val;
                token = scanner->getNext();
                if (token.type == LBracket) {
                    AstExpression *index = nullptr;
                    buildExpression(nullptr, RBracket, EmptyToken, &index);
                    
                    AstArrayAccess *acc = new AstArrayAccess(name);
                    acc->setIndex(index);
                    output.push(acc);
                } else {
                    AstID *id = new AstID(name);
                    output.push(id);
                    scanner->rewind(token);
                }
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
            
            case EQ: opStack.push(new AstEQOp); break;
            case NEQ: opStack.push(new AstNEQOp); break;
            case GT: opStack.push(new AstGTOp); break;
            case LT: opStack.push(new AstLTOp); break;
            case GTE: opStack.push(new AstGTEOp); break;
            case LTE: opStack.push(new AstLTEOp); break;
            
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
    
    if (stmt == nullptr) {
        *dest = output.top();
    } else {
        AstExpression *expr = output.top();
        stmt->addExpression(expr);
    }
    
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

