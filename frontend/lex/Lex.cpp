#include <iostream>
#include <cctype>

#include <lex/Lex.hpp>

// The token debug function
Token::Token() {
    type = EmptyToken;
    id_val = "";
    i32_val = 0;
}

// The scanner functions
Scanner::Scanner(std::string input) {
    reader = std::ifstream(input.c_str());
    if (!reader.is_open()) {
        std::cout << "Unknown input file." << std::endl;
        error = true;
    }
}

Scanner::~Scanner() {
    reader.close();
}

void Scanner::rewind(Token token) {
    token_stack.push(token);
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
        
        if (next == '#') {
            while (next != '\n' && !reader.eof()) {
                next = reader.get();
            }
        }
        
        if (next == '\"') {
            if (inQuote) {
                Token str;
                str.type = String;
                str.id_val = buffer;
                
                buffer = "";
                inQuote = false;
                return str;
            } else {
                inQuote = true;
                continue;
            }
        }
        
        if (inQuote) {
            if (next == '\\') {
                next = reader.get();
                switch (next) {
                    case 'n': buffer += '\n'; break;
                    case 't': buffer += '\t'; break;
                    default: buffer += '\\' + next;
                }
            } else {
                buffer += next;
            }
            continue;
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
        case '.':
        case '=':
        case '(':
        case ')':
        case '[':
        case ']':
        case ',': 
        case '+': 
        case '-': 
        case '*': 
        case '/': 
        case '>':
        case '<': 
        case '!': return true;
    }
    return false;
}

TokenType Scanner::getKeyword() {
    if (buffer == "extern") return Extern;
    else if (buffer == "func") return Func;
    else if (buffer == "begin") return Begin;
    else if (buffer == "end") return End;
    else if (buffer == "return") return Return;
    else if (buffer == "int") return Int;
    else if (buffer == "str") return Str;
    else if (buffer == "if") return If;
    else if (buffer == "elif") return Elif;
    else if (buffer == "else") return Else;
    else if (buffer == "while") return While;
    else if (buffer == "loop") return Loop;
    else if (buffer == "for") return For;
    else if (buffer == "is") return Is;
    else if (buffer == "then") return Then;
    else if (buffer == "do") return Do;
    else if (buffer == "break") return Break;
    else if (buffer == "continue") return Continue;
    else if (buffer == "in") return In;
    return EmptyToken;
}

TokenType Scanner::getSymbol(char c) {
    switch (c) {
        case '\n': return Nl;
        case ';': return SemiColon;
        case ':': return Colon;
        case '(': return LParen;
        case ')': return RParen;
        case '[': return LBracket;
        case ']': return RBracket;
        case ',': return Comma;
        case '+': return Plus;
        case '-': return Minus;
        case '*': return Mul;
        case '/': return Div;
        
        case '>': {
            char c2 = reader.get();
            if (c2 == '=') {
                return GTE;
            } else {
                reader.unget();
                return GT;
            }
        } break;
        
        case '<': {
            char c2 = reader.get();
            if (c2 == '=') {
                return LTE;
            } else {
                reader.unget();
                return LT;
            }
        } break;
        
        case '=': {
            char c2 = reader.get();
            if (c2 == '=') {
                return EQ;
            } else {
                reader.unget();
                return Assign;
            }
        } break;
        
        case '!': {
            char c2 = reader.get();
            if (c2 == '=') {
                return NEQ;
            } else {
                reader.unget();
            }
        } break;
        
        case '.': {
            char c2 = reader.get();
            if (c2 == '.') {
                return Range;
            } else {
                reader.unget();
            }
        } break;
    }
    return EmptyToken;
}

bool Scanner::isInt() {
    for (char c : buffer) {
        if (!isdigit(c)) return false;
    }
    return true;
}

