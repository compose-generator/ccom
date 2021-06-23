// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include <gtest/gtest.h>
#include <parser/ast/TopLevelExprAST.h>
#include <analyzer/Analyzer.h>
#include "../util/FileReader.h"

struct AnalyzerParams {
    const std::string fileName;
    const bool isSingleStatement;
    const std::string errorMessage; // Empty error message for testing for no error
};

const AnalyzerParams ANALYZER_TEST_PARAMETERS[] = {
        {
                "conditional-section-valid",
                false,
                ""
        },
        {
                "single-statement-valid",
                true,
                ""
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {};

TEST_P(AnalyzerTests, TestParserWithExpectToken) {
    AnalyzerParams param = GetParam();
    // Load parser input and expected output
    FileReader reader = FileReader("test-files");
    std::string fileInput = reader.fileToString( "analyzer/" + param.fileName, "input-file.txt");
    std::string dataInput = reader.fileToString( "analyzer/" + param.fileName, "input-data.json");
    // Initialize analyzer
    JSONParser jsonParser = JSONParser(dataInput);
    Analyzer analyzer = Analyzer(false, fileInput, jsonParser, "//", "/*", "*/");
    // Execute test case
    try {
        analyzer.executeAnalysis();
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";
        SUCCEED();
    } catch (std::runtime_error &error) {
        if (param.errorMessage.length() == 0)
            FAIL() << "Expected no error, but got '" + std::string(error.what()) + "'";
        EXPECT_EQ(std::string(error.what()), param.errorMessage);
    }
}

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTests,
        AnalyzerTests,
        ::testing::ValuesIn(ANALYZER_TEST_PARAMETERS)
);