#pragma once

#include <LLIR/Base.hpp>
#include <LLIR/Type.hpp>

class IRBuilder {
public:
    explicit IRBuilder(Module *mod) {
        this->mod = mod;
    }
protected:
    Module *mod = nullptr;
};