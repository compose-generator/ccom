//
// Created by Marc on 22.05.2021.
//

#pragma once

class TopLevelExprAST {
public:
    enum Type { TOP_LEVEL_EXPR, CONTENT_EXPR, STMT_LST_EXPR };

    explicit TopLevelExprAST(): type(TOP_LEVEL_EXPR) {}
    Type GetType();
protected:
    explicit TopLevelExprAST(Type t): type(t) {}
private:
    Type type;
};
