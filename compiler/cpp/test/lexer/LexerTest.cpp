//
// Created by Marc on 30.05.2021.
//

#include <vector>
#include <lexer/Lexer.h>
#include <gtest/gtest.h>
#include "../util/LanguageDescriptor.h"
#include "../util/FileReader.h"

struct LexerParams {
    const std::string fileName;
    std::vector<TokenType> expectedTokensLineComment;
    std::vector<TokenType> expectedTokensBlockComment;
};

const std::vector<LanguageDescriptor> LANGUAGES {
    LanguageDescriptor("yml", "#", "", ""),
    LanguageDescriptor("cpp", "//", "/*", "*/"),
    LanguageDescriptor("html", "", "<!--", "-->"),
    LanguageDescriptor("py", "#", R"(""")", R"(""")")
};

const LexerParams LEXER_TEST_PARAMETERS[] = {
    {
            "brace-close",
            {
                TokenType::TOK_COM_LINE_IDEN,
                TokenType::TOK_BRACE_CLOSE
            },
            {
                TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                TokenType::TOK_BRACE_CLOSE
            }
    },
    {
            "brace-open",
            {
                TokenType::TOK_COM_LINE_IDEN,
                TokenType::TOK_BRACE_OPEN
            },
            {
                TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                TokenType::TOK_BRACE_OPEN
            }
    },
    {
            "dot",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_DOT
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_DOT
            }
    },
    {
            "equals",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_EQUALS
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_EQUALS
            }
    },
    {
            "false",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_FALSE
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_FALSE
            }
    },
    {
            "greater",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_GREATER
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_GREATER
            }
    },
    {
            "greater-equals",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_GREATER_EQUAL
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_GREATER_EQUAL
            }
    },
    {
            "has",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_HAS
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_HAS
            }
    },
    {
            "identifier",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_IDENTIFIER
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_IDENTIFIER
            }
    },
    {
            "if",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_IF
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_IF
            }
    },
    {
            "index",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_INDEX
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_INDEX
            }
    },
    {
            "less",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_LESS
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_LESS
            }
    },
    {
            "less-equals",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_LESS_EQUAL
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_LESS_EQUAL
            }
    },
    {
            "not",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_NOT
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_NOT
            }
    },
    {
            "not-equals",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_NOT_EQUALS
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_NOT_EQUALS
            }
    },
    {
            "or",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_OR
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_OR
            }
    },
    {
            "string",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_STRING
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_STRING
            }
    },
    {
            "true",
            {
                    TokenType::TOK_COM_LINE_IDEN,
                    TokenType::TOK_TRUE
            },
            {
                    TokenType::TOK_COM_BLOCK_IDEN_OPEN,
                    TokenType::TOK_TRUE
            }
    }
};

class LexerTests : public ::testing::TestWithParam<LexerParams> {};

TEST_P(LexerTests, TestLexerWithExpectToken) {
    LexerParams param = GetParam();
    FileReader reader = FileReader("test-files");
    // Loop through all languages
    for (LanguageDescriptor lang : LANGUAGES) {
        std::string input = reader.fileToString(param.fileName, param.fileName + "." + lang.getFileExtension());
        Lexer lexer = Lexer(false, input, lang.getComLineIden(), lang.getComBlockIdenOpen(), lang.getComBlockIdenClose());

        // Check if the language does not supports block comments -> check with line, otherwise with block comments
        if (!lang.getComLineIden().empty()) {
            // Loop through all expected line comment tokens
            for (TokenType expectedType : param.expectedTokensLineComment) {
                int actualType = lexer.getLookahead().getType();
                EXPECT_EQ(expectedType, actualType);
                lexer.advance();
            }
        } else {
            // Loop through all expected line comment tokens
            for (TokenType expectedType : param.expectedTokensBlockComment) {
                int actualType = lexer.getLookahead().getType();
                EXPECT_EQ(expectedType, actualType);
                lexer.advance();
            }
        }
    }
}

TEST(LexerTests, TestLexerAdvancedCpp) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("advanced", "advanced.cpp");
    Lexer lexer = Lexer(false, advancedInput, "//", "/*", "*/");

    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_NOT);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_INDEX);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_OR);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_NOT_EQUALS);
    lexer.expect(TOK_LESS_EQUAL);
    lexer.expect(TOK_GREATER);
    lexer.expect(TOK_LESS);
    lexer.expect(TOK_GREATER_EQUAL);
    lexer.expect(TOK_GREATER);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_GREATER);
    lexer.expect(TOK_FALSE);
    lexer.expect(TOK_TRUE);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_BLOCK_IDEN_OPEN);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_NOT);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_FALSE);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_BLOCK_IDEN_OPEN);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_NOT_EQUALS);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_TRUE);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE);
    lexer.expect(TOK_ARBITRARY);
}

TEST(LexerTests, TestLexerAdvancedPython) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("advanced", "advanced.py");
    Lexer lexer = Lexer(false, advancedInput, "#", "", "");

    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_NOT);
    lexer.expect(TOK_NOT_EQUALS);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_LESS);
    lexer.expect(TOK_LESS_EQUAL);
    lexer.expect(TOK_GREATER_EQUAL);
    lexer.expect(TOK_GREATER);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_TRUE);
    lexer.expect(TOK_OR);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_FALSE);
    lexer.expect(TOK_INDEX);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_ARBITRARY);
}

TEST(LexerTests, TestLexerAdvancedHTML) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("advanced", "advanced.html");
    Lexer lexer = Lexer(false, advancedInput, "", "<!--", "-->");

    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_BLOCK_IDEN_OPEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_NOT);
    lexer.expect(TOK_TRUE);
    lexer.expect(TOK_FALSE);
    lexer.expect(TOK_OR);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_NOT_EQUALS);
    lexer.expect(TOK_LESS);
    lexer.expect(TOK_LESS_EQUAL);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_GREATER);
    lexer.expect(TOK_GREATER_EQUAL);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_NUMBER);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_INDEX);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_BLOCK_IDEN_OPEN);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE);
    lexer.expect(TOK_ARBITRARY);
}

TEST(LexerTests, TestLexerAdvancedYAML) {
    FileReader reader = FileReader("test-files");
    std::string advancedInput = reader.fileToString("advanced", "advanced.yml");
    Lexer lexer = Lexer(false, advancedInput, "#", "", "");

    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_INDEX);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_EQUALS);
    lexer.expect(TOK_FALSE);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_HAS);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_IF);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_INDEX);
    lexer.expect(TOK_DOT);
    lexer.expect(TOK_IDENTIFIER);
    lexer.expect(TOK_NOT_EQUALS);
    lexer.expect(TOK_STRING);
    lexer.expect(TOK_BRACE_OPEN);
    lexer.expect(TOK_ARBITRARY);
    lexer.expect(TOK_COM_LINE_IDEN);
    lexer.expect(TOK_BRACE_CLOSE);
}

INSTANTIATE_TEST_SUITE_P(
        LexerTests,
        LexerTests,
        ::testing::ValuesIn(LEXER_TEST_PARAMETERS)
);