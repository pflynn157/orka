#include <iostream>
#include <cctype>

#include <lex.hpp>

// The token debug function
Token::Token() {
    type = EmptyToken;
    id_val = "";
    i32_val = 0;
}

void Token::print() {
    switch (type) {
        case EmptyToken: std::cout << "?? "; break;
        case Eof: std::cout << "EOF "; break;
        
        case Func: std::cout << "FUNC "; break;
        case Begin: std::cout << "BEGIN "; break;
        case End: std::cout << "END "; break;
        case Return: std::cout << "RETURN "; break;
        
        case Id: std::cout << "ID "; break;
        case Int32: std::cout << "I32 "; break;
        
        default: {}
    }
    
    std::cout << id_val << " ";
    std::cout << i32_val << " ";
    
    std::cout << std::endl;
}

// The scanner functions
Scanner::Scanner(std::string input) {
    reader = std::ifstream(input.c_str());
    if (!reader.is_open()) {
        error = true;
    }
}

Scanner::~Scanner() {
    reader.close();
}

// The main scanning function
Token Scanner::getNext() {
    Token token;
    if (reader.eof()) {
        token.type = Eof;
        return token;
    }
    
    for (;;) {
        char next = reader.get();
        
        if (reader.eof()) {
            token.type = Eof;
            break;
        }
        
        if (next == ' ' || next == '\n') {
            if (buffer.length() == 0) continue;
            
            token.type = getKeyword();
            if (token.type != EmptyToken) {
                buffer = "";
                break;
            }
            
            // Reset everything
            buffer = "";
        } else {
            buffer += next;
        }
    }
    
    return token;
}

TokenType Scanner::getKeyword() {
    if (buffer == "func") return Func;
    else if (buffer == "begin") return Begin;
    else if (buffer == "end") return End;
    else if (buffer == "return") return Return;
    return EmptyToken;
}

