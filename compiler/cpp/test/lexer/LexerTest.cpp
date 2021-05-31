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
    LanguageDescriptor("py", "#", "", "")
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
    }
};

class LexerTests : public ::testing::TestWithParam<LexerParams> {};

TEST_P(LexerTests, TestLexerWithExpectToken) {
    LexerParams param = GetParam();
    FileReader reader = FileReader("test-files");
    // Loop through all languages
    for (LanguageDescriptor lang : LANGUAGES) {
        std::cout << "Test " << param.fileName << " for " << lang.getFileExtension() << std::endl;
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

INSTANTIATE_TEST_SUITE_P(
        LexerTests,
        LexerTests,
        ::testing::ValuesIn(LEXER_TEST_PARAMETERS)
);