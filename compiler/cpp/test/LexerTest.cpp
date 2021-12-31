/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include <vector>
#include <lexer/Lexer.h>
#include <gtest/gtest.h>
#include "util/LanguageDescriptor.h"
#include "util/FileReader.h"

struct LexerParams {
    const std::string fileName;
    std::map<std::string, std::vector<Token>> expectedTokensMap;
};

const std::vector<LanguageDescriptor> LANGUAGES {
    LanguageDescriptor("cpp", "//", "/*", "*/"),
    LanguageDescriptor("html", "", "<!--", "-->"),
    LanguageDescriptor("py", "#", R"(""")", R"(""")"),
    LanguageDescriptor("yml", "#", "", "")
};

const LexerParams LEXER_TEST_PARAMETERS[] = {
    {
            "brace-close",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_CLOSE, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_CLOSE, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_CLOSE, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_CLOSE, 1, 4)
                            }
                    }
            }
    },
    {
            "brace-open",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_OPEN, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_OPEN, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_OPEN, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_BRACE_OPEN, 1, 4)
                            }
                    }
            }
    },
    {
            "dot",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_DOT, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_DOT, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_DOT, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_DOT, 1, 4)
                            }
                    }
            }
    },
    {
            "equals",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_EQUALS, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_EQUALS, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_EQUALS, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_EQUALS, 1, 4)
                            }
                    }
            }
    },
    {
            "false",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_FALSE, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_FALSE, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_FALSE, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_FALSE, 1, 4)
                            }
                    }
            }
    },
    {
            "greater",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_GREATER, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER, 1, 4)
                            }
                    }
            }
    },
    {
            "greater-equal",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER_EQUAL, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_GREATER_EQUAL, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER_EQUAL, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_GREATER_EQUAL, 1, 4)
                            }
                    }
            }
    },
    {
            "has",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_HAS, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_HAS, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_HAS, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_HAS, 1, 4)
                            }
                    }
            }
    },
    {
            "contains",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_CONTAINS, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_CONTAINS, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_CONTAINS, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_CONTAINS, 1, 4)
                            }
                    }
            }
    },
    {
            "identifier",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IDENTIFIER, "testIdentifier", 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_IDENTIFIER, "testIdentifier", 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IDENTIFIER, "testIdentifier", 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IDENTIFIER, "testIdentifier", 1, 4)
                            }
                    }
            }
    },
    {
            "if",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IF, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_IF, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IF, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_IF, 1, 4)
                            }
                    }
            }
    },
    {
            "index",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_INDEX, "1234", 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_INDEX, "1234", 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_INDEX, "1234", 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_INDEX, "1234", 1, 4)
                            }
                    }
            }
    },
    {
            "less",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_LESS, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS, 1, 4)
                            }
                    }
            }
    },
    {
            "less-equal",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS_EQUAL, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_LESS_EQUAL, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS_EQUAL, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_LESS_EQUAL, 1, 4)
                            }
                    }
            }
    },
    {
            "not",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_NOT, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT, 1, 4)
                            }
                    }
            }
    },
    {
            "not-equals",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT_EQUALS, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_NOT_EQUALS, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT_EQUALS, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NOT_EQUALS, 1, 4)
                            }
                    }
            }
    },
    {
            "number",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NUMBER, "42", 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_NUMBER, "42", 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NUMBER, "42", 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_NUMBER, "42", 1, 4)
                            }
                    }
            }
    },
    {
            "or",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_OR, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_OR, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_OR, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_OR, 1, 4)
                            }
                    }
            }
    },
    {
            "string",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_STRING, "May the force be with you!", 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_STRING, "May the force be with you!", 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_STRING, "May the force be with you!", 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_STRING, "May the force be with you!", 1, 4)
                            }
                    }
            }
    },
    {
            "true",
            {
                    {
                            "cpp",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_TRUE, 1, 5)
                            }
                    },
                    {
                            "html",
                            {
                                    Token(TokenType::TOK_COM_BLOCK_IDEN_OPEN, 1, 1),
                                    Token(TokenType::TOK_TRUE, 1, 7)
                            }
                    },
                    {
                            "py",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_TRUE, 1, 4)
                            }
                    },
                    {
                            "yml",
                            {
                                    Token(TokenType::TOK_COM_LINE_IDEN, 1, 1),
                                    Token(TokenType::TOK_TRUE, 1, 4)
                            }
                    }
            }
    }
};

class LexerTests : public ::testing::TestWithParam<LexerParams> {};

TEST_P(LexerTests, TestLexerWithExpectToken) {
    LexerParams param = GetParam();
    FileReader reader = FileReader("test-files");
    // Loop through all languages
    for (auto lang : LANGUAGES) {
        std::string input = reader.fileToString( "lexer/" + param.fileName, param.fileName + "." + lang.getFileExtension());
        Lexer lexer = Lexer(false, input, lang.getComLineIden(), lang.getComBlockIdenOpen(), lang.getComBlockIdenClose());

        // Check if the expected tokens vector exists
        if (param.expectedTokensMap.find(lang.getFileExtension()) != param.expectedTokensMap.end()) {
            std::vector<Token> expectedTokens = param.expectedTokensMap[lang.getFileExtension()];
            for (Token expectedToken : expectedTokens) {
                EXPECT_EQ(expectedToken.getType(), lexer.getLookahead().getType());
                EXPECT_EQ(expectedToken.getValue(), lexer.getLookahead().getValue());
                EXPECT_EQ(expectedToken.getCodePos(), lexer.getLookahead().getCodePos());
                lexer.advance();
            }
        }
    }
}

void expectToken(Lexer& lexer, Token expectedToken) {
    Token actualToken = lexer.getLookahead();
    if (actualToken.getType() != expectedToken.getType() || actualToken.getValue() != expectedToken.getValue() ||
        actualToken.getCodePos() != expectedToken.getCodePos()) {
        throw std::runtime_error("Expected and actual tokens do not match at " + actualToken.getCodePos() +
            ". Expected was " + std::to_string(expectedToken.getType()) + ", actual was " +
            std::to_string(actualToken.getType()) + ", actual value was: '" + actualToken.getValue() + "'");
    }
    lexer.advance();
}

TEST(LexerTests, TestLexerAdvancedCpp) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("lexer/advanced", "advanced.cpp");
    Lexer lexer = Lexer(false, advancedInput, "//", "/*", "*/");

    std::string expectedValue = "// Copyright (c) Marc Auberer 2021. All rights reserved.\n"
                    "\n"
                    "//\n"
                    "// Created by Marc on 02.05.2021.\n"
                    "//\n"
                    "\n"
                    "#include \"Token.h\"\n"
                    "\n"
                    "Token::Token() = default;\n"
                    "\n"
                    "Token::Token(int type, unsigned int lineNum, unsigned int colNum) {\n"
                    "    this->type = type;\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 1, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 13, 5));
    expectToken(lexer, Token(TOK_IF, 13, 9));
    expectToken(lexer, Token(TOK_HAS, 13, 12));
    expectToken(lexer, Token(TOK_NOT, 13, 16));
    expectToken(lexer, Token(TOK_DOT, 13, 20));
    expectToken(lexer, Token(TOK_NUMBER, "42", 13, 22));
    expectToken(lexer, Token(TOK_INDEX, "142", 13, 25));
    expectToken(lexer, Token(TOK_STRING, "Identifier (not)", 13, 31));
    expectToken(lexer, Token(TOK_STRING, "Keyword (not)", 13, 50));
    expectToken(lexer, Token(TOK_IDENTIFIER, "realIdentifier", 13, 66));
    expectToken(lexer, Token(TOK_OR, 13, 81));
    expectToken(lexer, Token(TOK_EQUALS, 13, 83));
    expectToken(lexer, Token(TOK_NOT_EQUALS, 13, 86));
    expectToken(lexer, Token(TOK_LESS_EQUAL, 13, 89));
    expectToken(lexer, Token(TOK_GREATER, 13, 91));
    expectToken(lexer, Token(TOK_LESS, 13, 93));
    expectToken(lexer, Token(TOK_GREATER_EQUAL, 13, 94));
    expectToken(lexer, Token(TOK_GREATER, 13, 96));
    expectToken(lexer, Token(TOK_EQUALS, 13, 98));
    expectToken(lexer, Token(TOK_GREATER, 13, 100));
    expectToken(lexer, Token(TOK_FALSE, 13, 102));
    expectToken(lexer, Token(TOK_TRUE, 13, 108));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 13, 113));
    expectedValue = " this->lineNum = lineNum;\n"
                    "     this->colNum = colNum;\n"
                    "     {\n"
                    "     }\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 14, 5));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 18, 5));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 18, 9));
    expectedValue = "\n"
                    "}\n"
                    "\n"
                    "Token::Token(int type, std::string value, unsigned int lineNum, unsigned int colNum) {\n"
                    "    this->type = type;\n"
                    "    this->value = std::move(value);\n"
                    "    this->lineNum = lineNum;\n"
                    "    this->colNum = colNum;\n"
                    "}\n"
                    "\n"
                    "int Token::getType() const {\n"
                    "    return type;\n"
                    "}\n"
                    "\n"
                    "std::string Token::getValue() {\n"
                    "    return value;\n"
                    "}\n"
                    "\n"
                    "std::string Token::getCodePos() {\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 18, 10));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_OPEN, 37, 5));
    expectToken(lexer, Token(TOK_NUMBER, "23492873", 38, 6));
    expectToken(lexer, Token(TOK_STRING, "string", 38, 15));
    expectToken(lexer, Token(TOK_IDENTIFIER, "iden", 38, 24));
    expectToken(lexer, Token(TOK_NOT, 38, 29));
    expectToken(lexer, Token(TOK_EQUALS, 38, 33));
    expectToken(lexer, Token(TOK_HAS, 38, 36));
    expectToken(lexer, Token(TOK_FALSE, 38, 40));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 38, 46));
    expectedValue = "return \"Line \" + std::to_string(lineNum) + \", Col \" + std::to_string(colNum);\n"
                    "     ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 39, 6));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 40, 6));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_CLOSE, 40, 7));
    expectedValue = "\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 40, 9));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_OPEN, 41, 5));
    expectToken(lexer, Token(TOK_NUMBER, "123", 42, 6));
    expectToken(lexer, Token(TOK_STRING, "string", 42, 10));
    expectToken(lexer, Token(TOK_IDENTIFIER, "iden", 42, 19));
    expectToken(lexer, Token(TOK_NOT_EQUALS, 42, 24));
    expectToken(lexer, Token(TOK_HAS, 42, 27));
    expectToken(lexer, Token(TOK_TRUE, 42, 31));
    expectToken(lexer, Token(TOK_CONTAINS, 42, 36));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 43, 6));
    expectedValue = "return \"Line \" + std::to_string(lineNum) +\n"
                    "            \", Col \" + std::to_string(colNum);\n"
                    "     ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 44, 6));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 46, 6));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_CLOSE, 46, 7));
    expectedValue = "\n"
                    "}";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 46, 9));
}

TEST(LexerTests, TestLexerAdvancedPython) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("lexer/advanced", "advanced.py");
    Lexer lexer = Lexer(false, advancedInput, "#", "", "");

    std::string expectedValue = "from os.path import isfile, isdir, join\n"
                                "from os import listdir\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 1, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 3, 1));
    expectToken(lexer, Token(TOK_IF, 3, 4));
    expectToken(lexer, Token(TOK_HAS, 3, 7));
    expectToken(lexer, Token(TOK_NOT, 3, 11));
    expectToken(lexer, Token(TOK_NOT_EQUALS, 3, 15));
    expectToken(lexer, Token(TOK_EQUALS, 3, 18));
    expectToken(lexer, Token(TOK_LESS, 3, 21));
    expectToken(lexer, Token(TOK_LESS_EQUAL, 3, 23));
    expectToken(lexer, Token(TOK_GREATER_EQUAL, 3, 26));
    expectToken(lexer, Token(TOK_GREATER, 3, 29));
    expectToken(lexer, Token(TOK_STRING, "s", 3, 31));
    expectToken(lexer, Token(TOK_IDENTIFIER, "test", 3, 35));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 3, 40));
    expectedValue = " from cerberus import Validator\n"
                    " import sys\n"
                    " import yaml\n"
                    " import json\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 4, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 8, 1));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 8, 4));
    expectedValue = "\n"
                    "\n"
                    "def checkFileExistence():\n"
                    "    print('Checking file existence ...', end='')\n"
                    "    status = True\n"
                    "    template_path = '../../../predefined-services'\n"
                    "    template_types = [f for f in listdir(template_path) if isdir(join(template_path, f))]\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 8, 5));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 15, 5));
    expectToken(lexer, Token(TOK_IF, 15, 8));
    expectToken(lexer, Token(TOK_TRUE, 15, 11));
    expectToken(lexer, Token(TOK_OR, 15, 16));
    expectToken(lexer, Token(TOK_DOT, 15, 18));
    expectToken(lexer, Token(TOK_FALSE, 15, 19));
    expectToken(lexer, Token(TOK_INDEX, "756", 15, 25));
    expectToken(lexer, Token(TOK_DOT, 15, 31));
    expectToken(lexer, Token(TOK_DOT, 15, 32));
    expectToken(lexer, Token(TOK_DOT, 15, 33));
    expectToken(lexer, Token(TOK_CONTAINS, 15, 35));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 15, 44));
    expectedValue = " for template_type in template_types:\n"
                    "             template_type_path = template_path + '/' + template_type\n"
                    "             services = [f for f in listdir(template_type_path) if isdir(join(template_type_path, f))]\n"
                    "             for service in services:\n"
                    "                 service_path = template_type_path + '/' + service\n"
                    "                  Check if service exists\n"
                    "                 if not isfile(service_path + '/service.yml'):\n"
                    "                     print('service.yml is missing for following service: ' + service)\n"
                    "                     status = False\n"
                    "                  Check if config file exists\n"
                    "                 if not isfile(service_path + '/config.json'):\n"
                    "                     print('config.json is missing for following service: ' + service)\n"
                    "                     status = False\n"
                    "                  Check if README exists\n"
                    "                 if not isfile(service_path + '/README.md'):\n"
                    "                     print('README.md is missing for following service: ' + service)\n"
                    "                     status = False\n"
                    "    ";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 16, 5));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 33, 5));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 33, 8));
    expectedValue = "\n"
                    "    print(' done')\n"
                    "    return status";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 33, 9));
}

TEST(LexerTests, TestLexerAdvancedHTML) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("lexer/advanced", "advanced.html");
    Lexer lexer = Lexer(false, advancedInput, "", "<!--", "-->");

    std::string expectedValue = "<html>\n"
                                "<head>\n"
                                "\n"
                                "</head>\n"
                                "\n"
                                "<body>\n"
                                "<p>This is a <strong>Test</strong>.\n"
                                "<p>\n"
                                "<div>\n"
                                "    <p>asdfjklö ASDFJKLÖ #+-'test' \"Test\" [2].3 4<5<6>=1>0 { test } }--</p>\n"
                                "    <!-- This is a standard comment, NO payload-->\n"
                                "</div>\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 1, 1));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_OPEN, 13, 1));
    expectToken(lexer, Token(TOK_IF, 14, 1));
    expectToken(lexer, Token(TOK_HAS, 14, 4));
    expectToken(lexer, Token(TOK_NOT, 14, 8));
    expectToken(lexer, Token(TOK_TRUE, 14, 12));
    expectToken(lexer, Token(TOK_FALSE, 14, 17));
    expectToken(lexer, Token(TOK_OR, 14, 23));
    expectToken(lexer, Token(TOK_EQUALS, 14, 25));
    expectToken(lexer, Token(TOK_NOT_EQUALS, 14, 28));
    expectToken(lexer, Token(TOK_LESS, 14, 31));
    expectToken(lexer, Token(TOK_LESS_EQUAL, 14, 32));
    expectToken(lexer, Token(TOK_NUMBER, "36", 14, 35));
    expectToken(lexer, Token(TOK_GREATER, 14, 37));
    expectToken(lexer, Token(TOK_GREATER_EQUAL, 14, 39));
    expectToken(lexer, Token(TOK_IDENTIFIER, "testIdentifier", 14, 42));
    expectToken(lexer, Token(TOK_NUMBER, "42", 14, 57));
    expectToken(lexer, Token(TOK_STRING, "TestString", 14, 60));
    expectToken(lexer, Token(TOK_NUMBER, "73", 14, 73));
    expectToken(lexer, Token(TOK_DOT, 14, 75));
    expectToken(lexer, Token(TOK_INDEX, "124", 14, 77));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 14, 82));
    expectedValue = "<div>\n"
                    "    <p>This is my payload (including div)</p>\n"
                    "</div>\n"
                    "{\n"
                    "}\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 15, 1));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 20, 1));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_CLOSE, 20, 2));
    expectedValue = "\n"
                    "\n"
                    "</body>\n"
                    "\n"
                    "<footer>\n"
                    "    <p>Test</p>";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 20, 5));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_OPEN, 25, 16));
    expectToken(lexer, Token(TOK_IDENTIFIER, "nice", 25, 21));
    expectToken(lexer, Token(TOK_STRING, "string", 25, 25));
    expectToken(lexer, Token(TOK_CONTAINS, 25, 33));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 25, 41));
    expectedValue = "<div>\n"
                    "        <p>This is even more payload</>p>\n"
                    "    </div>";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 25, 42));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 27, 11));
    expectToken(lexer, Token(TOK_COM_BLOCK_IDEN_CLOSE, 27, 12));
    expectedValue = "\n"
                    "</footer>\n"
                    "\n"
                    "</html>";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 27, 15));
}

TEST(LexerTests, TestLexerAdvancedYAML) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("lexer/advanced", "advanced.yml");
    Lexer lexer = Lexer(false, advancedInput, "#", "", "");

    std::string expectedValue = "build: ${{SPRING_MAVEN_SOURCE_DIRECTORY}}\n"
                                "container_name: ${{PROJECT_NAME_CONTAINER}}-backend-spring-maven\n"
                                "restart: always\n"
                                "networks:\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 1, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 5, 1));
    expectToken(lexer, Token(TOK_IF, 5, 4));
    expectToken(lexer, Token(TOK_IDENTIFIER, "service", 5, 7));
    expectToken(lexer, Token(TOK_DOT, 5, 14));
    expectToken(lexer, Token(TOK_IDENTIFIER, "frontend", 5, 15));
    expectToken(lexer, Token(TOK_INDEX, "1", 5, 23));
    expectToken(lexer, Token(TOK_DOT, 5, 26));
    expectToken(lexer, Token(TOK_IDENTIFIER, "preselected", 5, 27));
    expectToken(lexer, Token(TOK_EQUALS, 5, 39));
    expectToken(lexer, Token(TOK_FALSE, 5, 42));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 5, 48));
    expectedValue = "  - frontend-backend\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 6, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 7, 1));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 7, 4));
    expectedValue = "\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 7, 5));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 8, 1));
    expectToken(lexer, Token(TOK_IF, 8, 4));
    expectToken(lexer, Token(TOK_HAS, 8, 7));
    expectToken(lexer, Token(TOK_IDENTIFIER, "service", 8, 11));
    expectToken(lexer, Token(TOK_DOT, 8, 18));
    expectToken(lexer, Token(TOK_IDENTIFIER, "database", 8, 19));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 8, 28));
    expectedValue = "  - backend-database\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 9, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 10, 1));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 10, 4));
    expectedValue = "\n"
                    "ports:\n"
                    "  - ${{SPRING_MAVEN_PORT}}:8080\n"
                    "env_file:\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 10, 5));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 14, 1));
    expectToken(lexer, Token(TOK_IF, 14, 4));
    expectToken(lexer, Token(TOK_IDENTIFIER, "service", 14, 7));
    expectToken(lexer, Token(TOK_DOT, 14, 14));
    expectToken(lexer, Token(TOK_IDENTIFIER, "frontend", 14, 15));
    expectToken(lexer, Token(TOK_CONTAINS, 14, 24));
    expectToken(lexer, Token(TOK_IDENTIFIER, "name", 14, 33));
    expectToken(lexer, Token(TOK_NOT_EQUALS, 14, 38));
    expectToken(lexer, Token(TOK_STRING, "spring-gradle", 14, 41));
    expectToken(lexer, Token(TOK_BRACE_OPEN, 14, 57));
    expectedValue = "   - environment.env\n";
    expectToken(lexer, Token(TOK_ARBITRARY, expectedValue, 15, 1));
    expectToken(lexer, Token(TOK_COM_LINE_IDEN, 16, 1));
    expectToken(lexer, Token(TOK_BRACE_CLOSE, 16, 4));
}

INSTANTIATE_TEST_SUITE_P(
        LexerTests,
        LexerTests,
        ::testing::ValuesIn(LEXER_TEST_PARAMETERS)
);