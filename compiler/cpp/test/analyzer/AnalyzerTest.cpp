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
                "conditional-section-error-int-string", // Expects int, but data has string
                false,
                "Incompatible data types: '\"Test string\"' is not an integer"
        },
        {
                "conditional-section-error-int-boolean", // Expects int, but data has boolean
                false,
                "Incompatible data types: 'false' is not an integer"
        },
        {
                "conditional-section-error-boolean-int", // Expects boolean, but data has int
                false,
                "Incompatible data types: '4321' is not a boolean"
        },
        {
                "conditional-section-error-boolean-string", // Expects boolean, but data has string
                false,
                "Incompatible data types: '\"Test string\"' is not a boolean"
        },
        {
                "conditional-section-error-string-int", // Expects string, but data has int
                false,
                "Incompatible data types: '1234' is not a string"
        },
        {
                "conditional-section-error-string-boolean", // Expects string, but data has boolean
                false,
                "Incompatible data types: 'true' is not a string"
        },
        {
                "single-statement-valid",
                true,
                ""
        },
        {
                "conditional-section-error-int-string", // Expects int, but data has string
                true,
                "Incompatible data types: '\"Test string\"' is not an integer"
        },
        {
                "conditional-section-error-int-boolean", // Expects int, but data has boolean
                true,
                "Incompatible data types: 'false' is not an integer"
        },
        {
                "conditional-section-error-boolean-int", // Expects boolean, but data has int
                true,
                "Incompatible data types: '4321' is not a boolean"
        },
        {
                "conditional-section-error-boolean-string", // Expects boolean, but data has string
                true,
                "Incompatible data types: '\"Test string\"' is not a boolean"
        },
        {
                "conditional-section-error-string-int", // Expects string, but data has int
                true,
                "Incompatible data types: '1234' is not a string"
        },
        {
                "conditional-section-error-string-boolean", // Expects string, but data has boolean
                true,
                "Incompatible data types: 'true' is not a string"
        }
};

class AnalyzerTests : public ::testing::TestWithParam<AnalyzerParams> {
};

TEST_P(AnalyzerTests, TestAnalyzerWithValidAndInvalidTestFiles) {
    AnalyzerParams param = GetParam();
    // Load parser input and expected output
    FileReader reader = FileReader("test-files");
    std::string fileInput = reader.fileToString("analyzer/" + param.fileName, "input-file.txt");
    std::string dataInput = reader.fileToString("analyzer/" + param.fileName, "input-data.json");
    // Initialize analyzer
    JSONParser jsonParser = JSONParser(dataInput);
    Analyzer analyzer = Analyzer(false, fileInput, jsonParser, "//", "/*", "*/");
    // Execute test case
    try {
        analyzer.executeAnalysis();
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";
        SUCCEED();
    } catch (IncompatibleTypesException &error) {
        if (param.errorMessage.length() == 0)
            FAIL() << "Expected no error, but got '" + std::string(error.what()) + "'";
        EXPECT_EQ(std::string(error.what()), param.errorMessage);
    } catch (std::runtime_error &error) {
        FAIL() << "Unknown error thrown: " + std::string(error.what());
    }
}

INSTANTIATE_TEST_SUITE_P(
        AnalyzerTests,
        AnalyzerTests,
        ::testing::ValuesIn(ANALYZER_TEST_PARAMETERS)
);