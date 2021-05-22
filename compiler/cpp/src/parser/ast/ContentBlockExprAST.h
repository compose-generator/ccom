//
// Created by Marc on 22.05.2021.
//

#pragma once

class ContentBlockExprAST {
public:
    enum Type { CONTENT_BLOCK_EXPR, SECTION_EXPR, ARBITRARY_EXPR };

    explicit ContentBlockExprAST(): type(CONTENT_BLOCK_EXPR) {}
    Type GetType();
protected:
    explicit ContentBlockExprAST(Type t): type(t) {}
private:
    Type type;
};
