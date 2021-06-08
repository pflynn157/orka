#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a conditional statement
bool Parser::buildConditional(AstBlock *block) {
    AstIfStmt *cond = new AstIfStmt;
    if (!buildExpression(cond, Then)) return false;
    block->addStatement(cond);

    ++layer;
    buildBlock(cond->getBlock(), layer, cond);
    
    return true;
}

// Builds an ELIF statement
bool Parser::buildElif(AstIfStmt *block) {
    AstElifStmt *elif = new AstElifStmt;
    if (!buildExpression(elif, Then)) return false;
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
    if (!buildExpression(loop, Do)) return false;
    block->addStatement(loop);
    
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

