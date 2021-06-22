// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "ContentBlockExprAST.h"
#include "SectionExprAST.h" // This import cant be in the header file for some reason
#include "ArbitraryExprAST.h" // This import cant be in the header file for some reason

ContentBlockExprType ContentBlockExprAST::getType() {
    return type;
}

std::string ContentBlockExprAST::serialize() {
    switch(type) {
        case SECTION_EXPR: {
            auto* sectionExpr = static_cast<SectionExprAST*>(this);
            return sectionExpr->serialize();
        }
        case ARBITRARY_EXPR: {
            auto* arbitraryExpr = static_cast<ArbitraryExprAST*>(this);
            return arbitraryExpr->serialize();
        }
        default:
            throw std::runtime_error("Unknown content block type");
    }
}
