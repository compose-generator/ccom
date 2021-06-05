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
        std::string input = reader.fileToString(param.fileName, param.fileName + "." + lang.getFileExtension());
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

void expectToken(Lexer lexer, Token expectedToken) {
    Token actualToken = lexer.getLookahead();
    if (actualToken.getType() != expectedToken.getType() || actualToken.getValue() != expectedToken.getValue() ||
        actualToken.getCodePos() != expectedToken.getCodePos()) {
        throw std::runtime_error("Expected and actual tokens do not match at " + actualToken.getCodePos());
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