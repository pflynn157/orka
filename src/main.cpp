#include <iostream>
#include <string>
#include <cstdio>

#include <preproc/Preproc.hpp>
#include <parser/Parser.hpp>
#include <ast.hpp>

#include <Compiler.hpp>

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
    
    std::string newInput = preprocessFile(input);
    if (newInput == "") {
        return 1;
    }
    
    Parser *frontend = new Parser(newInput);
    AstTree *tree;
    
    if (testLex) {
        frontend->debugScanner();
        return 0;
    }
    
    if (!frontend->parse()) {
        delete frontend;
        return 1;
    }
    
    tree = frontend->getTree();
    
    delete frontend;
    remove(newInput.c_str());
    
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

