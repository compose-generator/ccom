// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "ContentExprAST.h"

std::vector<std::unique_ptr<ExprAST>> const &ContentExprAST::GetSections() {
    return Sections;
}

std::vector<std::unique_ptr<SectionExprAST>> ContentExprAST::GetRelevantSections() {
    std::vector<std::unique_ptr<SectionExprAST>> relevantSections;
    for(const std::unique_ptr<ExprAST>& section : Sections) {
        if (auto* sectionExpr = dynamic_cast<SectionExprAST*>(section.get())) {
            relevantSections.push_back(std::make_unique<SectionExprAST>(std::move(*sectionExpr)));
        }
    }
    return relevantSections;
}