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
            case Extern:
            case Func: {
                code = buildFunction(token);
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
            case VarD: code = buildVariableDec(block); break;
            
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
            case Repeat: code = buildRepeat(block); break;
            case For: code = buildFor(block); break;
            case ForAll: code = buildForAll(block); break;
            
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
bool Parser::buildExpression(AstStatement *stmt, DataType currentType, TokenType stopToken, TokenType separateToken, AstExpression **dest) {
    std::stack<AstExpression *> output;
    std::stack<AstExpression *> opStack;
    
    DataType varType = currentType;

    Token token = scanner->getNext();
    while (token.type != Eof && token.type != stopToken) {
        if (token.type == separateToken && output.size() > 0) {
            AstExpression *expr = output.top();
            output.pop();
            
            if (stmt == nullptr) {
                if ((*dest)->getType() == AstType::FuncCallExpr) {
                    AstFuncCallExpr *fc = static_cast<AstFuncCallExpr *>(*dest);
                    fc->addArgument(expr);
                } else {
                    *dest = expr;
                }
            } else {
                stmt->addExpression(expr);
            }
            continue;
        }
    
        switch (token.type) {
            case True: {
                output.push(new AstBool(1));
            } break;
            
            case False: {
                output.push(new AstBool(0));
            } break;
            
            case CharL: {
                AstChar *c = new AstChar(token.i8_val);
                output.push(c);
            } break;
            
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
                if (varType == DataType::Void) {
                    varType = typeMap[name].first;
                    if (varType == DataType::Array) varType = typeMap[name].second;
                }
                
                token = scanner->getNext();
                if (token.type == LBracket) {
                    AstExpression *index = nullptr;
                    buildExpression(nullptr, DataType::Int32, RBracket, EmptyToken, &index);
                    
                    AstArrayAccess *acc = new AstArrayAccess(name);
                    acc->setIndex(index);
                    output.push(acc);
                } else if (token.type == LParen) {
                    AstFuncCallExpr *fc = new AstFuncCallExpr(name);
                    AstExpression *fcExpr = fc;
                    buildExpression(nullptr, varType, RParen, Comma, &fcExpr);
                    
                    output.push(fc);
                } else {
                    AstID *id = new AstID(name);
                    output.push(id);
                    scanner->rewind(token);
                }
            } break;
            
            case Sizeof: {
                std::string name = token.id_val;
                
                Token token1 = scanner->getNext();
                Token token2 = scanner->getNext();
                Token token3 = scanner->getNext();
                
                if (token1.type != LParen || token2.type != Id || token3.type != RParen) {
                    syntax->addError(scanner->getLine(), "Invalid token in sizeof.");
                    token.print();
                    return false;
                }
                
                AstID *id = new AstID(token2.id_val);
                AstSizeof *size = new AstSizeof(id);
                output.push(size);
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
            
            case Step: {
                if (stmt->getType() != AstType::For) {
                    syntax->addError(scanner->getLine(), "Step is only valid with for loops");
                    return false;
                }
                
                token = scanner->getNext();
                if (token.type != Int32) {
                    syntax->addError(scanner->getLine(), "Expected integer literal with \"step\"");
                    return false;
                }
                
                AstForStmt *forStmt = static_cast<AstForStmt *>(stmt);
                forStmt->setStep(token.i32_val);
            } break;
            
            default: {}
        }
        
        token = scanner->getNext();
    }
    
    // Build the expression
    while (opStack.size() > 0) {
        AstExpression *rval = checkExpression(output.top(), varType);
        output.pop();
        
        AstExpression *lval = checkExpression(output.top(), varType);
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
    
    // Type check the top
    AstExpression *expr = checkExpression(output.top(), varType);
    
    if (stmt == nullptr) {
        if ((*dest) == nullptr) {
            *dest = expr;
        } else if ((*dest)->getType() == AstType::FuncCallExpr) {
            AstFuncCallExpr *fc = static_cast<AstFuncCallExpr *>(*dest);
            fc->addArgument(expr);
        } else {
            *dest = expr;
        }
    } else {
        stmt->addExpression(expr);
    }
    
    return true;
}

// This is meant mainly for literals; it checks to make sure all the types in
// the expression agree in type. LLVM will have a problem if not
AstExpression *Parser::checkExpression(AstExpression *expr, DataType varType) {
    switch (expr->getType()) {
        case AstType::IntL: {
            // Change to byte literals
            if (varType == DataType::Byte || varType == DataType::UByte) {
                AstInt *i32 = static_cast<AstInt *>(expr);
                AstByte *byte = new AstByte(i32->getValue());
                expr = byte;
                
            // Change to word literals
            } else if (varType == DataType::Short || varType == DataType::UShort) {
                AstInt *i32 = static_cast<AstInt *>(expr);
                AstWord *i16 = new AstWord(i32->getValue());
                expr = i16;
                
            // Change to qword literals
            } else if (varType == DataType::Int64 || varType == DataType::UInt64) {
                AstInt *i32 = static_cast<AstInt *>(expr);
                AstQWord *i64 = new AstQWord(i32->getValue());
                expr = i64;
            }
        } break;
            
        default: {}
    }
    
    return expr;
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

