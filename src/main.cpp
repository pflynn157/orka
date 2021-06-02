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
    
    // Compiler (codegen) flags
    CFlags flags;
    flags.name = "a.out";
    
    // Other flags
    std::string input = "";
    bool testLex = false;
    bool printAst = false;
    bool printLLVM = false;
    
    for (int i = 1; i<argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--test-lex") {
            testLex = true;
        } else if (arg == "--ast") {
            printAst = true;
        } else if (arg == "--llvm") {
            printLLVM = true;
        } else if (arg == "-o") {
            flags.name = argv[i+1];
            i += 1;
        } else if (arg[0] == '-') {
            std::cerr << "Invalid option: " << arg << std::endl;
            return 1;
        } else {
            input = arg;
        }
    }
    
    Frontend *frontend = new Frontend(input);
    AstTree *tree;
    
    if (testLex) {
        frontend->debugScanner();
        return 0;
    }
    
    frontend->parse();
    tree = frontend->getTree();
    
    delete frontend;
    
    if (printAst) {
        tree->print();
        return 0;
    }

    //test
    Compiler *compiler = new Compiler(tree, flags);
    compiler->compile();
    
    if (printLLVM) {
        compiler->debug();
        return 0;
    }
    
    compiler->writeAssembly();
    compiler->assemble();
    compiler->link();
    
    return 0;
}

