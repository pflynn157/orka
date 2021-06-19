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
    bool buildFunction(Token startToken);
    bool buildVariableDec(AstBlock *block);
    bool buildVariableAssign(AstBlock *block, Token idToken);
    bool buildArrayAssign(AstBlock *block, Token idToken);
    bool buildFunctionCallStmt(AstBlock *block, Token idToken);
    bool buildReturn(AstBlock *block);
    bool buildConditional(AstBlock *block);
    bool buildElif(AstIfStmt *block);
    bool buildElse(AstIfStmt *block);
    bool buildWhile(AstBlock *block);
    bool buildLoop(AstBlock *block);
    bool buildFor(AstBlock *block);
    bool buildLoopCtrl(AstBlock *block, bool isBreak);
    bool buildBlock(AstBlock *block, int stopLayer = 0, AstIfStmt *parentBlock = nullptr, bool inElif = false);
    bool buildExpression(AstStatement *stmt, TokenType stopToken = SemiColon, TokenType separateToken = EmptyToken, AstExpression **dest = nullptr);
private:
    std::string input = "";
    Scanner *scanner;
    AstTree *tree;
    ErrorManager *syntax;
    int layer = 0;
};

