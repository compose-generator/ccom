//
// Created by Marc on 04.05.2021.
//

#ifndef COMPILER_STRINGEXPRAST_H
#define COMPILER_STRINGEXPRAST_H

#include <string>
#include <utility>

class StringExprAST {
private:
    std::string Val;
public:
    explicit StringExprAST(std::string val): Val(std::move(val)) {}
};

#endif //COMPILER_STRINGEXPRAST_H
