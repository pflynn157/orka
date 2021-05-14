#pragma once

#include <fstream>
#include <string>

// Represents a token
enum TokenType {
    EmptyToken,
    Eof,
    
    // Keywords
    Func,
    Begin,
    End,
    Return,
    
    // Literals
    Id,
    Int32
};

struct Token {
    TokenType type;
    std::string id_val;
    int i32_val;
    
    Token();
    void print();
};

// The main lexical analysis class
class Scanner {
public:
    explicit Scanner(std::string input);
    ~Scanner();
    
    Token getNext();
    
    bool isError() { return error; }
private:
    std::ifstream reader;
    bool error = false;
    
    // Control variables for the scanner
    std::string buffer = "";
    
    // Functions
    TokenType getKeyword();
};

