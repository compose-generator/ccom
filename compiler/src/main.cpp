//
// Created by Marc on 02.05.2021.
//

#include <iostream>
#include <vector>
#include "main.h"
#include "parser/parser.h"

// CLI call: ./ccom <file-input> <data-input> <line-com-chars> <block-com-open-chars> <block-com-close-chars>
int main(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    std::string fileInput = args[1];
    std::string dataInput = args[2];

    // Replace file input with test string
    fileInput = "property1: value\n//! if has frontend | has service.backend | var.FlaskPort = 8080 {\n//!! test payload\n//! }\nattribute2: value2";

    initParser(fileInput, dataInput, args[3], args[4], args[5]);

    return 0;
}