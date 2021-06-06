#include <compiler.hpp>

// Translates an AST IF statement to LLVM
void Compiler::compileIfStatement(AstStatement *stmt) {
    AstIfStmt *condStmt = static_cast<AstIfStmt *>(stmt);
    bool hasBranches = condStmt->getBranches().size();

    BasicBlock *trueBlock = BasicBlock::Create(*context, "true" + std::to_string(blockCount), currentFunc);
    BasicBlock *falseBlock = nullptr;
    BasicBlock *endBlock = BasicBlock::Create(*context, "end" + std::to_string(blockCount), currentFunc);
    if (hasBranches) falseBlock = BasicBlock::Create(*context, "false" + std::to_string(blockCount), currentFunc);
    ++blockCount;

    Value *cond = compileValue(stmt->getExpressions().at(0));
    if (hasBranches) builder->CreateCondBr(cond, trueBlock, falseBlock);
    else builder->CreateCondBr(cond, trueBlock, endBlock);

    // Align the blocks
    BasicBlock *current = builder->GetInsertBlock();
    trueBlock->moveAfter(current);

    if (hasBranches) {
        falseBlock->moveAfter(trueBlock);
        endBlock->moveAfter(falseBlock);
    } else {
        endBlock->moveAfter(trueBlock);
    }

    builder->SetInsertPoint(trueBlock);
    for (auto stmt : condStmt->getBlock()->getBlock()) {
        compileStatement(stmt);
    }
    builder->CreateBr(endBlock);

    // Branches
    bool hadElif = false;
    bool hadElse = false;

    for (auto stmt : condStmt->getBranches()) {
        if (stmt->getType() == AstType::Elif) {
            AstElifStmt *elifStmt = static_cast<AstElifStmt *>(stmt);
            
            BasicBlock *trueBlock2 = BasicBlock::Create(*context, "true" + std::to_string(blockCount), currentFunc);
            BasicBlock *falseBlock2 = BasicBlock::Create(*context, "false" + std::to_string(blockCount), currentFunc);
            
            // Align
            if (!hadElif) builder->SetInsertPoint(falseBlock);
            BasicBlock *current = builder->GetInsertBlock();
            trueBlock2->moveAfter(current);
            falseBlock2->moveAfter(trueBlock2);
            
            Value *cond = compileValue(stmt->getExpressions().at(0));
            builder->CreateCondBr(cond, trueBlock2, falseBlock2);
            
            builder->SetInsertPoint(trueBlock2);
            for (auto stmt2 : elifStmt->getBlock()->getBlock()) {
                compileStatement(stmt2);
            }
            builder->CreateBr(endBlock);
            
            builder->SetInsertPoint(falseBlock2);
            hadElif = true;
        } else if (stmt->getType() == AstType::Else) {
            AstElseStmt *elseStmt = static_cast<AstElseStmt *>(stmt);
            
            if (!hadElif) builder->SetInsertPoint(falseBlock);
            
            for (auto stmt2 : elseStmt->getBlock()->getBlock()) {
                compileStatement(stmt2);
            }
            builder->CreateBr(endBlock);
            
            hadElse = true;
        }
    }

    if (hadElif && !hadElse) {
        builder->CreateBr(endBlock);
    }

    // Start at the end block
    builder->SetInsertPoint(endBlock);
}

// Translates a while statement to LLVM
void Compiler::compileWhileStatement(AstStatement *stmt) {
    AstWhileStmt *loop = static_cast<AstWhileStmt *>(stmt);

    BasicBlock *loopBlock = BasicBlock::Create(*context, "loop_body" + std::to_string(blockCount), currentFunc);
    BasicBlock *loopCmp = BasicBlock::Create(*context, "loop_cmp" + std::to_string(blockCount), currentFunc);
    BasicBlock *loopEnd = BasicBlock::Create(*context, "loop_end" + std::to_string(blockCount), currentFunc);
    ++blockCount;

    BasicBlock *current = builder->GetInsertBlock();
    loopBlock->moveAfter(current);
    loopCmp->moveAfter(loopBlock);
    loopEnd->moveAfter(loopCmp);

    builder->CreateBr(loopCmp);
    builder->SetInsertPoint(loopCmp);
    Value *cond = compileValue(stmt->getExpressions().at(0));
    builder->CreateCondBr(cond, loopBlock, loopEnd);

    builder->SetInsertPoint(loopBlock);
    for (auto stmt : loop->getBlock()->getBlock()) {
        compileStatement(stmt);
    }
    builder->CreateBr(loopCmp);
    builder->SetInsertPoint(loopEnd);
}

