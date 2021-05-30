//
// Created by Marc on 30.05.2021.
//

//#include <vector>
//#include <lexer/Lexer.h>
//#include <gtest/gtest.h>
//#include "../util/LanguageDescriptor.h"
//#include "../util/FileReader.h"
//
//std::vector<LanguageDescriptor> getLanguageDescriptors() {
//    std::vector<LanguageDescriptor> languages;
//    languages.emplace_back("yml", "#", "", "");
//    languages.emplace_back("cpp", "//", "/*", "*/");
//    languages.emplace_back("html", "", "<!--", "-->");
//    languages.emplace_back("py", "#", "", "");
//    return languages;
//}
//
//class LexerTestEnvironment : public ::testing::Environment {
//public:
//    virtual void SetUp() {
//        // Initialize file reader
//        fileReader = FileReader("../test-files");
//        // Initialize language descriptors
//
//    }
//
//    std::vector<Lexer> getLexers(const std::string& fileName, bool singleStatement) {
//        std::vector<Lexer> lexers;
//        for (LanguageDescriptor l : languages) {
//            // Load input file
//            std::string lexerInput = fileReader.fileToString(fileName, fileName + "." + l.getFileExtension());
//            // Construct lexer
//            lexers.emplace_back(Lexer(singleStatement, lexerInput, l.getComLineIden(), l.getComBlockIdenOpen(),
//                                      l.getComBlockIdenClose()));
//        }
//        return lexers;
//    }
//};
//
//TEST(LexerTests, Dot) {
//    for (Lexer lexer : LexerTestEnvironment::getLexers("dot", false)) {
//        Token tok = lexer.getLookahead();
//        lexer.expect(TOK_DOT);
//
//    }
//}