#pragma once

#include <vector>
#include <string>

enum StmtExprType {
    STMT_EXPR,
    HAS_STMT_EXPR,
    COMP_STMT_EXPR,
    CONTAINS_STMT_EXPR
};

class StmtExprAST {
public:
    // Constructors
    explicit StmtExprAST(): type(STMT_EXPR) {}

    // Public methods
    StmtExprType getType();
    std::string serialize();
protected:
    // Protected constructors
    explicit StmtExprAST(StmtExprType t): type(t) {}
private:
    // Members
    StmtExprType type;
};
