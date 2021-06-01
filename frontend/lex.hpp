#pragma once

#include <fstream>
#include <string>
#include <stack>

// Represents a token
enum TokenType {
    EmptyToken,
    Eof,
    
    // Keywords
    Extern,
    Func,
    Begin,
    End,
    Return,
    Int,
    Str,
    
    // Literals
    Id,
    String,
    Int32,
    
    // Symbols
    Nl,
    SemiColon,
    Colon,
    Assign,
    LParen,
    RParen,
    Comma,
    
    Plus,
    Minus,
    Mul,
    Div
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
    
    void rewind(Token token);
    Token getNext();
    
    bool isError() { return error; }
private:
    std::ifstream reader;
    bool error = false;
    std::stack<Token> token_stack;
    
    // Control variables for the scanner
    std::string buffer = "";
    bool inQuote = false;
    
    // Functions
    bool isSymbol(char c);
    TokenType getKeyword();
    TokenType getSymbol(char c);
    bool isInt();
};

