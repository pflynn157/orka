#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a conditional statement
bool Parser::buildConditional(AstBlock *block) {
    AstIfStmt *cond = new AstIfStmt;
    if (!buildExpression(cond, DataType::Void, Then)) return false;
    block->addStatement(cond);

    ++layer;
    buildBlock(cond->getBlock(), layer, cond);
    
    return true;
}

// Builds an ELIF statement
bool Parser::buildElif(AstIfStmt *block) {
    AstElifStmt *elif = new AstElifStmt;
    if (!buildExpression(elif, DataType::Void, Then)) return false;
    block->addBranch(elif);
    
    buildBlock(elif->getBlock(), layer, block, true);
    return true;
}

// Builds an ELSE statement
bool Parser::buildElse(AstIfStmt *block) {
    AstElseStmt *elsee = new AstElseStmt;
    block->addBranch(elsee);
    
    buildBlock(elsee->getBlock(), layer);
    return true;
}

// Builds a while statement
bool Parser::buildWhile(AstBlock *block) {
    AstWhileStmt *loop = new AstWhileStmt;
    if (!buildExpression(loop, DataType::Void, Do)) return false;
    block->addStatement(loop);
    
    ++layer;
    buildBlock(loop->getBlock(), layer);
    
    return true;
}

// Builds an infinite loop statement
bool Parser::buildLoop(AstBlock *block) {
    AstLoopStmt *loop = new AstLoopStmt;
    block->addStatement(loop);
    
    ++layer;
    buildBlock(loop->getBlock(), layer);

    return true;
}

// Builds a for loop
bool Parser::buildFor(AstBlock *block) {
    AstForStmt *loop = new AstForStmt;
    block->addStatement(loop);
    
    // Get the index
    Token token = scanner->getNext();
    if (token.type != Id) {
        syntax->addError(scanner->getLine(), "Expected variable name for index.");
        return false;
    }
    
    loop->setIndex(new AstID(token.id_val));
    
    token = scanner->getNext();
    if (token.type != In) {
        syntax->addError(scanner->getLine(), "Expected \"in\".");
        return false;
    }
    
    // Build the starting and ending expression
    if (!buildExpression(loop, DataType::Void, Do, Range)) return false;
    
    if (loop->getExpressionCount() == 1) {
        AstExpression *start = loop->getExpressions().front();
        loop->clearExpressions();
        loop->setStartBound(start);
    } else if (loop->getExpressionCount() == 2) {
        AstExpression *end = loop->getExpressions().back();
        AstExpression *start = loop->getExpressions().front();
        
        loop->clearExpressions();
        
        loop->setStartBound(start);
        loop->setEndBound(end);
    } else {
        syntax->addError(scanner->getLine(), "Invalid expression in for loop.");
        return false;
    }
    
    ++layer;
    buildBlock(loop->getBlock(), layer);
    
    return true;
}

// Builds a loop keyword
bool Parser::buildLoopCtrl(AstBlock *block, bool isBreak) {
    if (isBreak) block->addStatement(new AstBreak);
    else block->addStatement(new AstContinue);
    
    Token token = scanner->getNext();
    if (token.type != SemiColon) {
        syntax->addError(scanner->getLine(), "Expected \';\' after break or continue.");
        return false;
    }
    
    return true;
}

