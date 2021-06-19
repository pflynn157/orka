#include <iostream>

#include <ast.hpp>

std::string printDataType(DataType dataType) {
    switch (dataType) {
        case DataType::Void: return "void";
        case DataType::Char: return "char";
        case DataType::Byte: return "byte";
        case DataType::UByte: return "ubyte";
        case DataType::Short: return "short";
        case DataType::UShort: return "ushort";
        case DataType::Int32: return "int32";
        case DataType::UInt32: return "uint32";
        case DataType::Int64: return "int64";
        case DataType::UInt64: return "uint64";
        case DataType::String: return "string";
        case DataType::Array: return "array";
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
    std::cout << "VAR_DEC " << name << " : " << printDataType(dataType);
    if (ptrType != DataType::Void) {
        std::cout << "*" << printDataType(ptrType);
        std::cout << "[";
        size->print();
        std::cout << "]";
    }
    std::cout << std::endl;
}

void AstVarAssign::print() {
    std::cout << "    ";
    std::cout << "VAR= " << name << " : " << printDataType(dataType);
    if (ptrType != DataType::Void) {
        std::cout << "*" << printDataType(ptrType);
        std::cout << "[]";
    }
    std::cout << std::endl;
}

void AstArrayAssign::print() {
    std::cout << "    ";
    std::cout << "ARR= " << name;
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

void AstElifStmt::print() {
    std::cout << "    ";
    std::cout << "ELIF" << std::endl;
    
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
    
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
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
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void AstLoopStmt::print() {
    std::cout << "    ";
    std::cout << "LOOP" << std::endl;
    
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
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
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void AstForStmt::print() {
    std::cout << "    ";
    std::cout << "FOR ";
    indexVar->print();
    std::cout << " IN ";
    startBound->print();
    if (endBound != nullptr) {
        std::cout << " .. ";
        endBound->print();
    }
    std::cout << std::endl;
    
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
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
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void AstBreak::print() {
    std::cout << "    ";
    std::cout << "BREAK" << std::endl;
}

void AstContinue::print() {
    std::cout << "    ";
    std::cout << "CONTINUE" << std::endl;
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

void AstEQOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") == (";
    rval->print();
    std::cout << ")";
}

void AstNEQOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") != (";
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

void AstGTEOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") >= (";
    rval->print();
    std::cout << ")";
}

void AstLTEOp::print() {
    std::cout << "(";
    lval->print();
    std::cout << ") <= (";
    rval->print();
    std::cout << ")";
}

void AstChar::print() {
    std::cout << "CHAR(" << val << ")";
}

void AstInt::print() {
    std::cout << val;
}

void AstQWord::print() {
    std::cout << val;
}

void AstString::print() {
    std::cout << "\"" << val << "\"";
}

void AstID::print() {
    std::cout << val;
}

void AstSizeof::print() {
    std::cout << "SIZEOF(";
    val->print();
    std::cout << ")";
}

void AstArrayAccess::print() {
    std::cout << val << "[";
    index->print();
    std::cout << "]";
}

void AstFuncCallExpr::print() {
    std::cout << name << "(";
    for (auto arg : args) {
        arg->print();
    }
    std::cout << ")";
}

