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
    } else {
        frontend->parse();
        tree = frontend->getTree();
    }
    delete frontend;
    
    tree->print();
    std::cout << "====================" << std::endl << std::endl;

    //test
    Compiler *compiler = new Compiler(tree);
    compiler->compile();
    compiler->debug();
    
    compiler->writeAssembly();
    compiler->assemble();
    compiler->link();
    
    return 0;
}

