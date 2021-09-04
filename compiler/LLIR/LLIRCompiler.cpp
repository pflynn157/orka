#include <LLIR/LLIRCompiler.hpp>
#include <LLIR/LLIR.hpp>
#include <LLIR/PASM.hpp>

LLIRCompiler::LLIRCompiler(AstTree *tree, std::string name) {
    this->tree = tree;
    this->name = name;
}

void LLIRCompiler::compile() {
    /*LLIRFile *file = new LLIRFile("output1");
    LLIRType *voidType = new LLIRType(LLIRDataType::Void);
    LLIRType *i32 = new LLIRType(LLIRDataType::I32);
    
    LLIRFunction *func = new LLIRFunction("main", voidType);
    file->addFunction(func);
    
    LLIRBlock *block = new LLIRBlock("entry");
    func->setEntryBlock(block);
    
    LLIRImm *val = new LLIRImm(23, i32);
    LLIRInstruction *ret = new LLIRInstruction("", i32, LLIR::Ret);
    ret->setArg1(val);
    block->addInstruction(ret);
    
    //db
    file->dump();*/
    
    // PASM
    PASMFile *pasm = new PASMFile("output1");
    
    PASMLabel *mainLbl = new PASMLabel("main");
    pasm->addCode(mainLbl);
    
    PASMInstruction *li = new PASMInstruction(PASM::LI);
    li->setOperand1(new PASMReg32(0));
    li->setOperand2(new PASMImm(23));
    pasm->addCode(li);
    
    PASMInstruction *ret = new PASMInstruction(PASM::RET);
    pasm->addCode(ret);
    
    pasm->debug();
}

void LLIRCompiler::debug() {

}

