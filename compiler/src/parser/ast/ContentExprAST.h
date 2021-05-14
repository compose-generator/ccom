// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_CONTENTEXPRAST_H
#define COMPILER_CONTENTEXPRAST_H

#include <vector>
#include <memory>
#include <algorithm>
#include "ExprAST.h"
#include "SectionExprAST.h"

class ContentExprAST : public ExprAST {
private:
    std::vector<std::unique_ptr<ExprAST>> Sections;
public:
    explicit ContentExprAST(std::vector<std::unique_ptr<ExprAST>> sections): Sections(std::move(sections)) {}
    const std::vector<std::unique_ptr<ExprAST>> &GetSections();
};

#endif //COMPILER_CONTENTEXPRAST_H
