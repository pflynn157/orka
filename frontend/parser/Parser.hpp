#pragma once

#include <string>

#include <lex/Lex.hpp>
#include <ast.hpp>

// The parser class
// The parser is in charge of performing all parsing and AST-building tasks
// It is also in charge of the error manager

class Parser {
public:
    explicit Parser(std::string input);
    ~Parser();
    
    void parse();
    
    AstTree *getTree() { return tree; }
    
    void debugScanner();
protected:
    void buildFunction();
    void buildExternFunction();
    void buildVariableDec(AstFunction *func, Token idToken);
    void buildVariableAssign(AstFunction *func, Token idToken);
    void buildFunctionCallStmt(AstFunction *func, Token idToken);
    void buildReturn(AstFunction *func);
    void buildExpression(AstStatement *stmt, TokenType stopToken = SemiColon, TokenType separateToken = EmptyToken);
private:
    std::string input = "";
    Scanner *scanner;
    AstTree *tree;
};

