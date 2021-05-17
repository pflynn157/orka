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
}

