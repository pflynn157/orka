#include "llvm/IR/BasicBlock.h"

using namespace llvm;

#include <compiler.hpp>

Compiler::Compiler(AstTree *tree) {
    this->tree = tree;

    context = std::make_unique<LLVMContext>();
    mod = std::make_unique<Module>("mod1", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
}

void Compiler::compile() {
    for (auto global : tree->getGlobalStatements()) {
        switch (global->getType()) {
            case AstType::Func: {
                AstFunction *astFunc = static_cast<AstFunction *>(global);

                FunctionType *FT = FunctionType::get(Type::getVoidTy(*context), Type::getVoidTy(*context), false);
                Function *func = Function::Create(FT, Function::ExternalLinkage, astFunc->getName(), mod.get());

                BasicBlock *mainBlock = BasicBlock::Create(*context, "entry", func);
                builder->SetInsertPoint(mainBlock);

                builder->CreateRetVoid();
            } break;

            default: {}
        }
    }
}

void Compiler::debug() {
    mod->print(errs(), nullptr);
}
