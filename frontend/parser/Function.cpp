#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Returns the function arguments
bool Parser::getFunctionArgs(std::vector<Var> &args) {
    Token token = scanner->getNext();
    if (token.type == LParen) {
        token = scanner->getNext();
        while (token.type != Eof && token.type != RParen) {
            Token t1 = token;
            Token t2 = scanner->getNext();
            Token t3 = scanner->getNext();
            Var v;
            v.subType = DataType::Void;
            
            if (t1.type != Id) {
                syntax->addError(scanner->getLine(), "Invalid function argument: Expected name.");
                return false;
            }
            
            if (t2.type != Colon) {
                syntax->addError(scanner->getLine(), "Invalid function argument: Expected \':\'.");
                return false;
            }
            
            switch (t3.type) {
                case Int: v.type = DataType::Int32; break;
                case Str: {
                    v.type = DataType::Ptr;
                    v.subType = DataType::Char;
                } break;
                
                default: {
                    syntax->addError(scanner->getLine(), "Invalid function argument: Unknown type.");
                    return false;
                }
            }
            
            v.name = t1.id_val;
            args.push_back(v);
            
            token = scanner->getNext();
            if (token.type == Comma) {
                token = scanner->getNext();
            }
        }
    } else {
        scanner->rewind(token);
    }
    
    return true;
}

// Builds a function
bool Parser::buildFunction() {
    Token token = scanner->getNext();
    std::string funcName = token.id_val;
    
    if (token.type != Id) {
        syntax->addError(scanner->getLine(), "Expected function name.");
        return false;
    }
    
    // Get arguments
    std::vector<Var> args;
    if (!getFunctionArgs(args)) return false;

    // Check to see if there's any return type
    token = scanner->getNext();
    bool isExtern = false;
    
    if (token.type == Is) {
        token = scanner->getNext();
        if (token.type == Extern) {
            isExtern = true;
        } else {
            scanner->rewind(token);
        }
    } else {
        syntax->addError(scanner->getLine(), "Expected \'is\' keyword.");
        return false;
    }

    // Create the function object
    if (isExtern) {
        AstExternFunction *ex = new AstExternFunction(funcName);
        ex->setArguments(args);
        tree->addGlobalStatement(ex);
        return true;
    }
    
    AstFunction *func = new AstFunction(funcName);
    func->setArguments(args);
    tree->addGlobalStatement(func);
    
    // Build the variable body
    token = scanner->getNext();
    while (token.type != Eof && token.type != Begin) {
        bool code = true;
        
        switch (token.type) {
            case Id: code = buildVariableDec(func->getBlock(), token); break;
            
            case Nl: break;
            
            default: {
                syntax->addError(scanner->getLine(), "Invalid token in expression.");
                return false;
            }
        }
        
        if (!code) return false;
        token = scanner->getNext();
    }
    
    // Build the body
    return buildBlock(func->getBlock());
}

// Builds a function call
bool Parser::buildFunctionCallStmt(AstBlock *block, Token idToken) {
    AstFuncCallStmt *fc = new AstFuncCallStmt(idToken.id_val);
    block->addStatement(fc);
    
    if (!buildExpression(fc, RParen, Comma)) return false;
    
    Token token = scanner->getNext();
    if (token.type != SemiColon) {
        syntax->addError(scanner->getLine(), "Expected \';\'.");
        token.print();
        return false;
    }
    
    return true;
}

// Builds a return statement
bool Parser::buildReturn(AstBlock *block) {
    AstReturnStmt *stmt = new AstReturnStmt;
    block->addStatement(stmt);
    
    if (!buildExpression(stmt)) return false;
    
    return true;
}

