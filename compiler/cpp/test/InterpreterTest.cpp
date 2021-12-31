/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include <string>
#include <gtest/gtest.h>
#include <interpreter/Interpreter.h>
#include "util/FileReader.h"

struct InterpreterParams {
    const std::string fileName;
    const std::string fileExt;
    const std::string lineComIden;
    const std::string blockComIdenOpen;
    const std::string blockComIdenClose;
    const bool isSingleStatement;
    const std::string errorMessage; // Empty error message for testing for no error
};

const InterpreterParams INTERPRETER_TEST_PARAMETERS[] = {
        {
                "case1",
                "yml",
                "#",
                "",
                "",
                false,
                ""
        },
        {
                "case2",
                "cpp",
                "//",
                "/*",
                "*/",
                false,
                ""
        },
        {
                "case3",
                "html",
                "",
                "<!--",
                "-->",
                false,
                ""
        },
        {
                "case4",
                "html",
                "",
                "<!--",
                "-->",
                false,
                ""
        },
        {
                "case5",
                "yml",
                "#",
                "",
                "",
                false,
                ""
        }
};

class InterpreterTests : public ::testing::TestWithParam<InterpreterParams> {};

TEST_P(InterpreterTests, TestInterpreterWithDifferentFiles) {
    InterpreterParams param = GetParam();
    // Load input, data and expected output
    FileReader reader = FileReader("test-files");
    std::string input = reader.fileToString("interpreter/" + param.fileName, "input." + param.fileExt);
    std::string data = reader.fileToString("interpreter/" + param.fileName, "data.json");
    std::string expectedOutput = reader.fileToString("interpreter/" + param.fileName, "output." + param.fileExt);
    // Initialize interpreter
    Interpreter interpreter = Interpreter(false, input, data, param.lineComIden, param.blockComIdenOpen, param.blockComIdenClose);
    // Execute test case
    try {
        std::string output = interpreter.interpretInput();
        if (param.errorMessage.length() > 0)
            FAIL() << "Expected error message '" + param.errorMessage + "', but got no error";
        SUCCEED();
    } catch (UnexpectedDataTypeException &error) {
        if (param.errorMessage.length() == 0)
            FAIL() << "Expected no error, but got '" + std::string(error.what()) + "'";
        EXPECT_EQ(std::string(error.what()), param.errorMessage);
    } catch (std::runtime_error &error) {
        FAIL() << "Unknown error thrown: " + std::string(error.what());
    }
}

INSTANTIATE_TEST_SUITE_P(
        InterpreterTests,
        InterpreterTests,
        ::testing::ValuesIn(INTERPRETER_TEST_PARAMETERS)
);