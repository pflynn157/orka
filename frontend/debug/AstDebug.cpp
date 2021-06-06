#include <iostream>

#include <ast.hpp>

std::string printDataType(DataType dataType) {
    switch (dataType) {
        case DataType::Void: return "void";
        case DataType::Char: return "char";
        case DataType::Int32: return "i32";
        case DataType::Ptr: return "ptr";
    }
    return "";
}

void AstTree::print() {
    std::cout << "FILE: " << file << std::endl;
    std::cout << std::endl;
    
    for (auto stmt : global_statements) {
        stmt->print();
    }
}

void AstExternFunction::print() {
    std::cout << "EXTERN FUNC " << name << "(";
    for (auto var : args) {
        std::cout << printDataType(var.type);
        if (var.subType != DataType::Void)
            std::cout << "*" << printDataType(var.subType);
        std::cout << ", ";
    }
    std::cout << ")" << std::endl;
}

void AstFunction::print() {
    std::cout << "FUNC " << name << "(";
    for (auto var : args) {
        std::cout << printDataType(var.type);
        if (var.subType != DataType::Void)
            std::cout << "*" << printDataType(var.subType);
        std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    
    for (auto stmt : block->getBlock()) {
        stmt->print();
        if (stmt->getExpressionCount()) {
            for (auto expr : stmt->getExpressions()) {
                for (int i = 0; i<8; i++) std::cout << " ";
                expr->print();
            }
            std::cout << std::endl;
        }
    }
}

void AstFuncCallStmt::print() {
    std::cout << "    ";
    std::cout << "FC " << name;
    std::cout << std::endl;
}

void AstReturnStmt::print() {
    std::cout << "    ";
    std::cout << "RETURN ";
    
    std::cout << std::endl;
}

void AstVarDec::print() {
    std::cout << "    ";
    std::cout << "VAR " << name << " : " << printDataType(dataType);
    std::cout << std::endl;
}

void AstVarAssign::print() {
    std::cout << "    ";
    std::cout << "VAR= " << name;
    std::cout << std::endl;
}

void AstIfStmt::print() {
    std::cout << "    ";
    std::cout << "IF " << std::endl;
    
    std::cout << "=========================" << std::endl;
    for (auto stmt : block->getBlock()) {
        stmt->print();
        if (stmt->getExpressionCount()) {
            for (auto expr : stmt->getExpressions()) {
                for (int i = 0; i<8; i++) std::cout << " ";
                expr->print();
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << ">>" << std::endl;
    
    for (auto stmt : branches) {
        stmt->print();
        if (stmt->getExpressionCount()) {
            for (auto expr : stmt->getExpressions()) {
                for (int i = 0; i<8; i++) std::cout << " ";
                expr->print();
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "=========================" << std::endl;
}

void AstElseStmt::print() {
    std::cout << "    ";
    std::cout << "ELSE" << std::endl;
    
    std::cout << "-------------------------" << std::endl;
    for (auto stmt : block->getBlock()) {
        stmt->print();
        if (stmt->getExpressionCount()) {
            for (auto expr : stmt->getExpressions()) {
                for (int i = 0; i<8; i++) std::cout << " ";
                expr->print();
            }
            std::cout << std::endl;
        }
    }
    std::cout << "-------------------------" << std::endl;
}

void AstWhileStmt::print() {
    std::cout << "    ";
    std::cout << "WHILE" << std::endl;
}

void AstEnd::print() {
    std::cout << "    ";
    std::cout << "END";
    
    std::cout << std::endl;
}

void AstAddOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") + (";
    rval->print();
    std::cout << ")";
}

void AstSubOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") - (";
    rval->print();
    std::cout << ")";
}

void AstMulOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") * (";
    rval->print();
    std::cout << ")";
}

void AstDivOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") / (";
    rval->print();
    std::cout << ")";
}

void AstGTOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") > (";
    rval->print();
    std::cout << ")";
}

void AstLTOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") < (";
    rval->print();
    std::cout << ")";
}

void AstInt::print() {
    std::cout << val;
}

void AstString::print() {
    std::cout << "\"" << val << "\"";
}

void AstID::print() {
    std::cout << val;
}

