// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "main.h"

// CLI call: ./ccom <file-input> <data-input> <line-com-chars> <block-com-open-chars> <block-com-close-chars>
int main(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    bool singleStatementMode = args[1] == "1";
    std::string fileInput = args[2];
    std::string dataInput = args[3];

    // Replace file input with test string
    //fileInput = "property1:= value\n//? if has frontend | test.Test == 90133 | var.FlaskPort == \"8\\\"080\\\"\" {\n// test payload}\n// - another test payload\n//? }\nattribute2: value2";
    //fileInput = "property1:= value\n/*? if has frontend | test.Test == 90133 | var.FlaskPort == \"8\\\"080\\\"\" {\ntest payload\n- }another test payload\n}*/\nattribute2: value2";
    fileInput = "<html>\n"
                "    <head>\n"
                "        <!--? if var.test == \"Title 1\" {\n"
                "        <title>Title 1</title>\n"
                "        }-->\n"
                "        <!--? if var.test == \"Title 2\" {\n"
                "        <title>Title 2</title>\n"
                "        }-->\n"
                "    </head>\n"
                "    <body>\n"
                "        <!--? if has var.body {\n"
                "        <h2>Body is available</h2><br>\n"
                "        <h3>Subheading</h3>\n"
                "        }-->\n"
                "    </body>\n"
                "</html>";

    initParser(singleStatementMode, fileInput, dataInput, args[4], args[5], args[6]);

    return 0;
}