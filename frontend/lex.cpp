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
        case Int: std::cout << "INT"; break;
        
        case Id: std::cout << "ID "; break;
        case Int32: std::cout << "I32 "; break;
        
        case Nl: std::cout << "\\n "; break;
        case SemiColon: std::cout << "; "; break;
        case Colon: std::cout << ": "; break;
        case Assign: std::cout << "= "; break;
        
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
    if (token_stack.size() > 0) {
        Token top = token_stack.top();
        token_stack.pop();
        return top;
    }

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
        
        if (next == ' ' || isSymbol(next)) {
            if (buffer.length() == 0) {
                if (isSymbol(next)) {
                    Token sym;
                    sym.type = getSymbol(next);
                    return sym;
                }
                continue;
            }
            
            // Check if we have a symbol
            if (isSymbol(next)) {
                Token sym;
                sym.type = getSymbol(next);
                token_stack.push(sym);
            }
            
            // Now check the buffer
            token.type = getKeyword();
            if (token.type != EmptyToken) {
                buffer = "";
                break;
            }
            
            if (isInt()) {
                token.type = Int32;
                token.i32_val = std::stoi(buffer);
            } else {
                token.type = Id;
                token.id_val = buffer;
            }
            
            // Reset everything
            buffer = "";
            break;
        } else {
            buffer += next;
        }
    }
    
    return token;
}

bool Scanner::isSymbol(char c) {
    switch (c) {
        case '\n':
        case ';': 
        case ':': 
        case '=': return true;
    }
    return false;
}

TokenType Scanner::getKeyword() {
    if (buffer == "func") return Func;
    else if (buffer == "begin") return Begin;
    else if (buffer == "end") return End;
    else if (buffer == "return") return Return;
    else if (buffer == "int") return Int;
    return EmptyToken;
}

TokenType Scanner::getSymbol(char c) {
    switch (c) {
        case '\n': return Nl;
        case ';': return SemiColon;
        case ':': return Colon;
        case '=': return Assign;
    }
    return EmptyToken;
}

bool Scanner::isInt() {
    for (char c : buffer) {
        if (!isdigit(c)) return false;
    }
    return true;
}

