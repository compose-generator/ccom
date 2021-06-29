// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include <string>
#include <gtest/gtest.h>
#include <interpreter/Interpreter.h>
#include "../util/FileReader.h"

struct InterpreterParams {
    const std::string fileName;
    const std::string fileExt;
    const std::string lineComIden;
    const std::string blockComIdenOpen;
    const std::string blockComIdenClose;
    const bool isSingleStatement;
    const std::string errorMessage; // Empty error message for testing for no error
};