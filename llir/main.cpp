#include <iostream>

#include <LLIR/Type.hpp>
#include <LLIR/Base.hpp>
#include <LLIR/IRBuilder.hpp>

int main(int argc, char **argv) {
    Module *mod = new Module("mod1");
    IRBuilder *builder = new IRBuilder(mod);
    
    // The function
    // --> void main(void)
    VoidType *voidTy = new VoidType();
    FunctionType *mainFuncTy = new FunctionType(voidTy);
    Function *mainFunc = new Function("main", mainFuncTy);
    mod->addFunction(mainFunc);
    
    Block *entryBlock = new Block("entry");
    mainFunc->addBlock(entryBlock);
    
    // The code
    //$ ld r1, 10
    //$ add r2, r1, 30
    //$ ret r2
    I32Type *i32Ty = new I32Type();
    Instruction *ld = new Instruction(InstrType::LdI, i32Ty, "r1");
    ld->setOperand(new ConstI32(10), 1);
    entryBlock->addInstruction(ld);
    
    Instruction *add = new Instruction(InstrType::Add, i32Ty, "r2");
    add->setOperand(ld, 1);
    add->setOperand(new ConstI32(30), 2);
    entryBlock->addInstruction(add);
    
    mod->dump();
    
    return 0;
}
