// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

class ValueExprAST {
public:
    enum Type { VALUE_EXPR, NUMBER_EXPR, BOOLEAN_EXPR, STRING_EXPR };

    explicit ValueExprAST(): type(VALUE_EXPR) {}
    Type GetType();
protected:
    explicit ValueExprAST(Type t): type(t) {}
private:
    Type type;
};
