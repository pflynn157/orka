#pragma once

#include <string>

#include <lex/Lex.hpp>
#include <error/Manager.hpp>
#include <ast.hpp>

// The parser class
// The parser is in charge of performing all parsing and AST-building tasks
// It is also in charge of the error manager

class Parser {
public:
    explicit Parser(std::string input);
    ~Parser();
    
    bool parse();
    
    AstTree *getTree() { return tree; }
    
    void debugScanner();
protected:
    bool getFunctionArgs(std::vector<Var> &args);
    bool buildFunction();
    bool buildExternFunction();
    bool buildVariableDec(AstFunction *func, Token idToken);
    bool buildVariableAssign(AstFunction *func, Token idToken);
    bool buildFunctionCallStmt(AstFunction *func, Token idToken);
    bool buildReturn(AstFunction *func);
    bool buildConditional(AstFunction *func);
    bool buildWhile(AstFunction *func);
    bool buildExpression(AstStatement *stmt, TokenType stopToken = SemiColon, TokenType separateToken = EmptyToken);
private:
    std::string input = "";
    Scanner *scanner;
    AstTree *tree;
    ErrorManager *syntax;
    int layer = 0;
};

