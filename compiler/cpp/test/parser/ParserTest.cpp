// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include <parser/Parser.h>
#include <gtest/gtest.h>
#include "../util/LanguageDescriptor.h"
#include "../../lib/json/json.hpp"

using json = nlohmann::json;

struct ParserParams {
    const std::string fileName;
};

const std::vector<LanguageDescriptor> LANGUAGES {
        LanguageDescriptor("cpp", "//", "/*", "*/"),
        LanguageDescriptor("html", "", "<!--", "-->"),
        LanguageDescriptor("py", "#", R"(""")", R"(""")"),
        LanguageDescriptor("yml", "#", "", "")
};

const ParserParams PARSER_TEST_PARAMETERS[] = {
        {
            "string-expr"
        }
};

class ParserTests : public ::testing::TestWithParam<ParserParams> {};

TEST_P(ParserTests, TestParserWithExpectToken) {
    ParserParams param = GetParam();

}

void compareASTs(const std::unique_ptr<TopLevelExprAST> expected, const std::unique_ptr<TopLevelExprAST> actual) {

}

INSTANTIATE_TEST_SUITE_P(
        ParserTests,
        ParserTests,
        ::testing::ValuesIn(PARSER_TEST_PARAMETERS)
);