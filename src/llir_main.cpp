#include <iostream>

#include <LLIR/Type.hpp>
#include <LLIR/Base.hpp>
#include <LLIR/IRBuilder.hpp>

#include <Amd64/AMD64_IR.hpp>
#include <Amd64/AMD64_Writer.hpp>

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
    
    Instruction *ret = new Instruction(InstrType::Ret, i32Ty, "", true);
    ret->setOperand(add, 1);
    entryBlock->addInstruction(ret);
    
    mod->dump();
    
    // Now write x86
    AMD64File *file = new AMD64File("first");
    
    AMD64Function *func = new AMD64Function("main", 0);
    file->addText(func);
    
    // mov eax, 10
    AMD64Reg32 *r1 = new AMD64Reg32(AMD64_R32::EAX);
    AMD64Const32 *const1 = new AMD64Const32(10);
    AMD64Mov *mov = new AMD64Mov(r1, const1);
    file->addText(mov);
    
    // mov [rbp-4], eax
    AMD64Mem *mem = new AMD64Mem(new AMD64Const32(4), 32);
    AMD64Mov *mov2 = new AMD64Mov(mem, r1);
    file->addText(mov2);
    
    // add eax, 30
    AMD64Const32 *const2 = new AMD64Const32(30);
    AMD64Add *addX86 = new AMD64Add(r1, const2);
    file->addText(addX86);
    
    // ret
    AMD64Ret *retX86 = new AMD64Ret;
    file->addText(retX86);
    
    AMD64Writer *writer = new AMD64Writer(file);
    writer->write();
    
    return 0;
}