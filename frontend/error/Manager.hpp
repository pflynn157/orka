#pragma once

#include <string>
#include <vector>

struct Error {
    int line;
    std::string message;
};

class ErrorManager {
public:
    void addError(int line, std::string message);
    void addWarning(int line, std::string message);
    bool errorsPresent();
    void printErrors();
    void printWarnings();
private:
    std::vector<Error> errors;
    std::vector<Error> warnings;
};

