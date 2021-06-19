#pragma once

#include <string>
#include <map>

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
    // Function.cpp
    bool getFunctionArgs(std::vector<Var> &args);
    bool buildFunction(Token startToken);
    bool buildFunctionCallStmt(AstBlock *block, Token idToken);
    bool buildReturn(AstBlock *block);
    
    // Variable.cpp
    bool buildVariableDec(AstBlock *block);
    bool buildVariableAssign(AstBlock *block, Token idToken);
    bool buildArrayAssign(AstBlock *block, Token idToken);
    
    // Flow.cpp
    bool buildConditional(AstBlock *block);
    bool buildElif(AstIfStmt *block);
    bool buildElse(AstIfStmt *block);
    bool buildWhile(AstBlock *block);
    bool buildLoop(AstBlock *block);
    bool buildFor(AstBlock *block);
    bool buildLoopCtrl(AstBlock *block, bool isBreak);
    
    bool buildBlock(AstBlock *block, int stopLayer = 0, AstIfStmt *parentBlock = nullptr, bool inElif = false);
    bool buildExpression(AstStatement *stmt, DataType currentType,
                        TokenType stopToken = SemiColon, TokenType separateToken = EmptyToken,
                        AstExpression **dest = nullptr);
    AstExpression *checkExpression(AstExpression *expr, DataType varType);
private:
    std::string input = "";
    Scanner *scanner;
    AstTree *tree;
    ErrorManager *syntax;
    int layer = 0;
    
    std::map<std::string, std::pair<DataType,DataType>> typeMap;
};

