#pragma once

#include <string>

#include <lex.hpp>

// The frontend class
// The frontend is in charge of performing all parsing and AST-building tasks
// It is also in charge of the error manager

class Frontend {
public:
    explicit Frontend(std::string input);
    ~Frontend();
    
    void debugScanner();
private:
    std::string input = "";
    Scanner *scanner;
};

