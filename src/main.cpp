#include <iostream>
#include <string>

#include <frontend.hpp>
#include <ast.hpp>

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
    
    tree->print();
    
    return 0;
}

