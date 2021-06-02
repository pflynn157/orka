#include <iostream>
#include <string>

#include <frontend.hpp>
#include <ast.hpp>

#include <compiler.hpp>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    std::string arg = "";
    if (argc == 3) arg = argv[2];
    
    Frontend *frontend = new Frontend(input);
    AstTree *tree;
    
    if (arg == "--test-lex") {
        frontend->debugScanner();
        return 0;
    }
    
    frontend->parse();
    tree = frontend->getTree();
    
    delete frontend;
    
    if (arg == "--ast") {
        tree->print();
        return 0;
    }

    //test
    Compiler *compiler = new Compiler(tree);
    compiler->compile();
    
    if (arg == "--llvm") {
        compiler->debug();
        return 0;
    }
    
    compiler->writeAssembly();
    compiler->assemble();
    compiler->link();
    
    return 0;
}

