#include <iostream>
#include <string>

#include <frontend.hpp>
#include <ast.hpp>
#include <llir.hpp>

#include <compiler.hpp>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    Frontend *frontend = new Frontend(input);
    frontend->parse();
    AstTree *tree = frontend->getTree();
    //frontend->debugScanner();
    delete frontend;
    
    //tree->print();
    
    // Test LLIR
    LLirFile *file = new LLirFile("first.qk");
    LLirFuncDefinition *def = new LLirFuncDefinition("main");
    file->addFunctionDefinition(def);
    
    LLirReturn *ret = new LLirReturn;
    ret->setOperand(new LLirInt32(10), 1);
    def->addCode(ret);
    
    file->writeOut("first.asm");
    
    // Test compile
    compile(file, "first_x86.asm");
    
    return 0;
}

