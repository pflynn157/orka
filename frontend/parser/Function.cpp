#include <iostream>

#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a function
bool Parser::buildFunction() {
    Token token = scanner->getNext();
    if (token.type != Id) {
        syntax->addError(scanner->getLine(), "Expected function name.");
        return false;
    }

    AstFunction *func = new AstFunction(token.id_val);
    tree->addGlobalStatement(func);
    
    // Build the variable body
    token = scanner->getNext();
    while (token.type != Eof && token.type != Begin) {
        switch (token.type) {
            case Id: buildVariableDec(func, token); break;
            
            case Nl: break;
            
            default: {
                syntax->addError(scanner->getLine(), "Invalid token in expression.");
                return false;
            }
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
                } else if (token.type == LParen) {
                    buildFunctionCallStmt(func, idToken);
                } else {
                    syntax->addError(scanner->getLine(), "Invalid use of identifier.");
                    return false;
                }
            } break;
            
            case Return: buildReturn(func); break;
            
            case End: 
            case Nl: break;
            
            default: {
                syntax->addError(scanner->getLine(), "Invalid token in expression.");
                return false;
            }
        }
        
        token = scanner->getNext();
    }
    
    return true;
}

// Builds an extern function declaration
void Parser::buildExternFunction() {
    Token token = scanner->getNext();
    if (token.type != Func) {
        std::cerr << "Error: Expected \"func\"." << std::endl;
        return;
    }
    
    token = scanner->getNext();
    if (token.type != Id) {
        std::cerr << "Error: Expected function name." << std::endl;
        return;
    }
    
    std::string funcName = token.id_val;
    std::vector<Var> args;
    
    token = scanner->getNext();
    if (token.type = LParen) {
        token = scanner->getNext();
        while (token.type != Eof && token.type != RParen) {
            Token t1 = token;
            Token t2 = scanner->getNext();
            Token t3 = scanner->getNext();
            Var v;
            
            if (t1.type != Id) {
                std::cerr << "Error: Invalid function argument: Expected name." << std::endl;
                return;
            }
            
            if (t2.type != Colon) {
                std::cerr << "Error: Invalid function argument: Expected \':\'." << std::endl;
                return;
            }
            
            switch (t3.type) {
                case Int: v.type = DataType::Int32; break;
                case Str: {
                    v.type = DataType::Ptr;
                    v.subType = DataType::Char;
                } break;
                
                default: {
                    std::cerr << "Error: Invalid function argument: Unknown type." << std::endl;
                    return;
                }
            }
            
            v.name = t1.id_val;
            args.push_back(v);
            
            token = scanner->getNext();
            if (token.type == Comma) {
                token = scanner->getNext();
            }
        }
    }
    
    AstExternFunction *ex = new AstExternFunction(funcName);
    ex->setArguments(args);
    tree->addGlobalStatement(ex);
}

// Builds a function call
void Parser::buildFunctionCallStmt(AstFunction *func, Token idToken) {
    AstFuncCallStmt *fc = new AstFuncCallStmt(idToken.id_val);
    func->addStatement(fc);
    
    buildExpression(fc, RParen, Comma);
    
    Token token = scanner->getNext();
    if (token.type != SemiColon) {
        std::cerr << "Error: Expected \';\'." << std::endl;
        return;
    }
}

// Builds a return statement
void Parser::buildReturn(AstFunction *func) {
    AstReturnStmt *stmt = new AstReturnStmt;
    func->addStatement(stmt);
    
    buildExpression(stmt);
}

