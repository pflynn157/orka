#include <parser/Parser.hpp>
#include <ast.hpp>

// Builds a conditional statement
bool Parser::buildConditional(AstBlock *block) {
    AstIfStmt *cond = new AstIfStmt;
    if (!buildExpression(cond, Nl)) return false;
    block->addStatement(cond);

    ++layer;
    buildBlock(cond->getBlock(), layer, cond);
    
    return true;
}

// Builds an ELIF statement
bool Parser::buildElif(AstIfStmt *block) {
    AstElifStmt *elif = new AstElifStmt;
    if (!buildExpression(elif, Nl)) return false;
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
    if (!buildExpression(loop, Nl)) return false;
    block->addStatement(loop);
    
    ++layer;
    buildBlock(loop->getBlock(), layer);
    
    return true;
}

