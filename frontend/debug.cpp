#include <iostream>

#include "ast.hpp"

void AstTree::print() {
    std::cout << "FILE: " << file << std::endl;
    std::cout << std::endl;
    
    for (auto stmt : global_statements) {
        stmt->print();
    }
}

void AstFunction::print() {
    std::cout << "FUNC: " << name << std::endl;
    
    for (auto stmt : code) {
        stmt->print();
        for (auto expr : stmt->getExpressions()) expr->print();
        std::cout << std::endl;
    }
}

void AstReturnStmt::print() {
    std::cout << "    ";
    std::cout << "RETURN ";
    
    std::cout << std::endl;
}

void AstInt::print() {
    for (int i = 0; i<8; i++) std::cout << " ";
    std::cout << val;
}

