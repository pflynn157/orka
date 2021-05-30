#pragma once

#include <string>

#include <lex.hpp>
#include <ast.hpp>

// The frontend class
// The frontend is in charge of performing all parsing and AST-building tasks
// It is also in charge of the error manager

class Frontend {
public:
    explicit Frontend(std::string input);
    ~Frontend();
    
    void parse();
    
    AstTree *getTree() { return tree; }
    
    void debugScanner();
protected:
    void buildFunction();
    void buildReturn(AstFunction *func);
    void buildExpression(AstStatement *stmt);
private:
    std::string input = "";
    Scanner *scanner;
    AstTree *tree;
};

