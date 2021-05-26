// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "main.h"

// CLI call: ./ccom <single-stmt> <file-input> <data-input> <com-line-iden> <com-block-open-iden> <com-block-close-iden>
int main1(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    bool singleStatementMode = args[1] == "true";
    std::string fileInput = args[2];
    std::string dataInput = args[3];
    std::string comLineIden = args[4];
    std::string comBlockOpenIden = args[5];
    std::string comBlockCloseIden = args[6];

    // Replace file input with test string
    //fileInput = "property1:= value\n//? if has frontend | test.Test == 90133 | var.FlaskPort == \"8\\\"080\\\"\" {\n// test payload}\n// - another test payload\n//? }\nattribute2: value2";
    //fileInput = "property1:= value\n/*? if has frontend | test.Test == 90133 | var.FlaskPort == \"8\\\"080\\\"\" {\ntest payload\n- }another test payload\n}*/\nattribute2: value2";
    //fileInput = "//? if not has test.marc | test.marc.dominic.24 != \"Test\" { Test payload //? }";

    //comLineIden = "";
    //comBlockOpenIden = "<!--";
    //comBlockCloseIden = "-->";

    // Start compiler pipeline
    Interpreter interpreter = Interpreter(singleStatementMode, fileInput, dataInput,
                                          comLineIden, comBlockOpenIden, comBlockCloseIden);
    std::string output = interpreter.interpretInput();

    // Print output
    std::cout << output;

    // Return with positive result code
    return 0;
}