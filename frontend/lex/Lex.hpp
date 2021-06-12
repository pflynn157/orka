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
    If,
    Elif,
    Else,
    While,
    Loop,
    For,
    Is,
    Then,
    Do,
    Break,
    Continue,
    In,
    Sizeof,
    
    // Datatype Keywords
    Char,
    Byte,
    Int,
    Str,
    
    // Literals
    Id,
    String,
    CharL,
    Int32,
    
    // Symbols
    Nl,
    SemiColon,
    Colon,
    Assign,
    LParen,
    RParen,
    LBracket,
    RBracket,
    Comma,
    Range,
    Arrow,
    
    Plus,
    Minus,
    Mul,
    Div,
    
    EQ,
    NEQ,
    GT,
    LT,
    GTE,
    LTE,
};

struct Token {
    TokenType type;
    std::string id_val;
    char i8_val;
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
    
    int getLine() { return 0; }
    
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
    bool isHex();
};

