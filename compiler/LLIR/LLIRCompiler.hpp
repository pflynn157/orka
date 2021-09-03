#pragma once

#include <string>

#include <ast.hpp>

class LLIRCompiler {
public:
    explicit LLIRCompiler(AstTree *tree, std::string name);
    void compile();
    void debug();
private:
    AstTree *tree;
    std::string name = "";
};

