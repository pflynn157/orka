#include <fstream>
#include <iostream>
#include <string>

#include <llir.hpp>

void LLirFile::writeOut(std::string output_path) {
    std::ofstream writer(output_path);
    writer << "#" << input_file << std::endl << std::endl;
    
    for (auto func : func_defs) {
        func->writeOut(writer);
    }
}

void LLirFuncDefinition::writeOut(std::ofstream &writer) {
    writer << "define " << name << ":" << std::endl;
    
    for (auto stmt : main_block) stmt->writeOut(writer);
}

void LLirReturn::writeOut(std::ofstream &writer) {
    writer << "  return ";
    if (op1 != nullptr) op1->writeOut(writer);
    writer << std::endl;
}

void LLirInt32::writeOut(std::ofstream &writer) {
    writer << val;
}

