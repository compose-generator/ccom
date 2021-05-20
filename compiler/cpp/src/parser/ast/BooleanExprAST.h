//
// Created by Marc on 20.05.2021.
//

#ifndef COMPILER_BOOLEANEXPRAST_H
#define COMPILER_BOOLEANEXPRAST_H

#include "ValueExprAST.h"

class BooleanExprAST : public ValueExprAST {
private:
    bool Value;
public:
    explicit BooleanExprAST(bool val): Value(val) {}
    bool GetValue() const;
};

#endif //COMPILER_BOOLEANEXPRAST_H
