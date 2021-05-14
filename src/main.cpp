#include <iostream>
#include <string>

#include <frontend.hpp>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No input file specified." << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    Frontend *frontend = new Frontend(input);
    frontend->debugScanner();
    delete frontend;
    
    return 0;
}

