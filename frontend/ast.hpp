#pragma once

#include <string>
#include <vector>

#include <ast/Types.hpp>
#include <ast/Global.hpp>
#include <ast/Statement.hpp>
#include <ast/Expression.hpp>

// Forward declarations
//class AstGlobalStatement;
class AstStatement;
class AstExpression;

// Represents an AST tree
class AstTree {
public:
    explicit AstTree(std::string file) { this-> file = file; }
    ~AstTree() {}
    
    std::vector<AstGlobalStatement *> getGlobalStatements() {
        return global_statements;
    }
    
    void addGlobalStatement(AstGlobalStatement *stmt) {
        global_statements.push_back(stmt);
    }
    
    void print();
private:
    std::string file = "";
    std::vector<AstGlobalStatement *> global_statements;
};

