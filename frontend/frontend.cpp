#include <iostream>

#include <frontend.hpp>

Frontend::Frontend(std::string input) {
    this->input = input;
    scanner = new Scanner(input);
}

Frontend::~Frontend() {
    delete scanner;
}

// The debug function for the scanner
void Frontend::debugScanner() {
    std::cout << "Debugging scanner..." << std::endl;
    
    Token t;
    do {
        t = scanner->getNext();
        t.print();
    } while (t.type != Eof);
}

