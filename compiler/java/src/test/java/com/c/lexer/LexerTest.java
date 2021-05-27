package com.c.lexer;

import com.c.LanguageDescription;
import com.c.TestFileReader;
import com.c.filereader.MaxLookAheadException;
import com.c.filereader.UnexpectedCharException;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.HashMap;

import static org.assertj.core.api.Assertions.assertThat;

public class LexerTest {

    private final TestFileReader testFileReader = new TestFileReader("lexer");
    private static final HashMap<String, LanguageDescription> languages = new HashMap<>();

    public LexerTest() {
        // Comment identifiers for different languages
        languages.put("Java", new LanguageDescription(".java", "//", "/*", "*/"));
        languages.put("Yaml", new LanguageDescription(".yml", "#", "", ""));
        languages.put("Html", new LanguageDescription(".html", "", "<!--", "-->"));
        languages.put("Python", new LanguageDescription(".py", "#", "'''", "'''"));
    }


    // ------------------------------------------- Util ----------------------------------------------------------------

    private interface LanguageCommentCallback {
        void doTestsWithLexer(Lexer lexer, LanguageDescription language) throws UnexpectedCharException, UnknownCharException, UnexpectedTokenException;
    }

    private void testForEveryLanguage(String filename, LanguageCommentCallback callback)
            throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException, UnexpectedTokenException {
        for (LanguageDescription language : languages.values()) {
            Lexer lexer = constructLexer(filename, language);
            callback.doTestsWithLexer(lexer, language);
        }
    }

    private Lexer constructLexer(String filename, LanguageDescription language) throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException {
        String file = testFileReader.fileToString(filename + language.getFileExtension());
        return new Lexer(
                file,
                language.getCommentLineIdentifier(),
                language.getCommentBlockOpenIdentifier(),
                language.getCommentBlockCloseIdentifier(),
                false
        );
    }


    // ----------------------------------- Check comment identifiers ---------------------------------------------------

    @Test
    @DisplayName("Provide neither line nor block comment identifier to lexer")
    public void testNeitherLineNorBlockCommentIdentifier() throws MaxLookAheadException, UnexpectedCharException, UnknownCharException {
        String file = "irrelevant test file";
        try {
            new Lexer(file, "", "", "", false);
        } catch (InvalidCommentsIdentifierException err) {
            assertThat(err.getMessage()).isEqualTo("You must provide at least line comments or block comments identifier");
        }
    }

    @Test
    @DisplayName("Provide only an open block comment identifier to lexer")
    public void testOnlyOpenBlockCommentIdentifier() throws MaxLookAheadException, UnexpectedCharException, UnknownCharException {
        String file = "irrelevant test file";
        try {
            new Lexer(file, "", ":(", "", false);
        } catch (InvalidCommentsIdentifierException err) {
            assertThat(err.getMessage()).isEqualTo("You provided an identifier to open block comments but not one to close them");
        }
    }

    @Test
    @DisplayName("Provide only a close block comment identifier to lexer")
    public void testOnlyCloseBlockCommentIdentifier() throws MaxLookAheadException, UnexpectedCharException, UnknownCharException {
        String file = "irrelevant test file";
        try {
            new Lexer(file, "", "", ":)", false);
        } catch (InvalidCommentsIdentifierException err) {
            assertThat(err.getMessage()).isEqualTo("You provided an identifier to close block comments but not one to open them");
        }
    }


    // ---------------------------- Look ahead + expect - individual token types ---------------------------------------

    private int checkCommentStart(Lexer lexer, LanguageDescription language) throws UnexpectedTokenException, UnexpectedCharException, UnknownCharException {
        TokenType type = !language.getCommentLineIdentifier().isEmpty() ? TokenType.COMMENT_LINE_IDENTIFIER : TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER;
        Token expectedStartToken = new Token(type, 1, 1);
        assertThat(lexer.lookAhead()).isEqualTo(expectedStartToken);
        lexer.expect(expectedStartToken);

        // +1 since we start counting from 1
        // +1 for the question mark after the comment identifier
        // +1 for the space after the comment identifiers in the test files
        return type == TokenType.COMMENT_LINE_IDENTIFIER
                ? language.getCommentLineIdentifier().length() + 3
                : language.getCommentBlockOpenIdentifier().length() + 3;
    }

    private void checkExpectToken(Lexer lexer, Token expectedToken) throws UnexpectedTokenException, UnexpectedCharException, UnknownCharException {
        assertThat(lexer.lookAhead()).isEqualTo(expectedToken);
        lexer.expect(expectedToken);
    }

    @Test
    @DisplayName("Test Token EOF in empty file")
    public void testTokenEOFInEmptyFile() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Empty", (lexer, language) -> {
            assertThat(lexer.lookAhead()).isEqualTo(new Token(TokenType.EOF, 1, 1));
        });
    }

    @Test
    @DisplayName("Test Token IF")
    public void testTokenIf() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("If", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.IF, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token HAS")
    public void testTokenHas() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Has", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.HAS, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token NOT")
    public void testTokenNot() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Not", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.NOT, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token TRUE")
    public void testTokenTrue() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("True", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.TRUE, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token FALSE")
    public void testTokenFalse() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("False", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.FALSE, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token OR (|)")
    public void testTokenOr() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Or", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.OR, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token EQUALS (==)")
    public void testTokenEquals() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Equals", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.EQUALS, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token NOT_EQUALS (!=)")
    public void testTokenNotEquals() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("NotEquals", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.NOT_EQUALS, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token LESS (<)")
    public void testTokenLess() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Less", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.LESS, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token LESS_EQUAL (<=)")
    public void testTokenLessOrEqual() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("LessEqual", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.LESS_EQUAL, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token GREATER (>)")
    public void testTokenGreater() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Greater", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.GREATER, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token GREATER_EQUAL (>=)")
    public void testTokenGreaterEqual() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("GreaterEqual", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.GREATER_EQUAL, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token IDENTIFIER")
    public void testTokenIdentifier() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Identifier", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "TestIdentifier", 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token NUMBER")
    public void testTokenNumber() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Number", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.NUMBER, "42", 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token STRING")
    public void testTokenString() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("String", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.STRING, "TestString", 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token DOT")
    public void testTokenDot() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Dot", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.DOT, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token BRACE_OPEN")
    public void testTokenBraceOpen() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("BraceOpen", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token BRACE_CLOSE")
    public void testTokenBraceClose() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("BraceClose", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 1, posCol));
        });
    }

    @Test
    @DisplayName("Test Token INDEX")
    public void testTokenIndex() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Index", (lexer, language) -> {
            int posCol = checkCommentStart(lexer, language);
            checkExpectToken(lexer, new Token(TokenType.INDEX, "42", 1, posCol));
        });
    }


    // ------------------------------ Context switches + whitespaces + multiple Tokens ---------------------------------

    @Test
    @DisplayName("Advanced HTML")
    void testAdvancedHtml() throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException, UnexpectedTokenException {
        Lexer lexer = constructLexer("Advanced", languages.get("Html"));

        // Arbitrary begin
        String arbitraryBegin = """
                <html>
                <head>

                </head>

                <body>
                <p>This is a <strong>Test</strong>.
                <p>
                <div>
                    <p>asdfjklö ASDFJKLÖ #+-'test' "Test" [2].3 4<5<6>=1>0 { test } }--</p>
                    <!-- This is a standard comment, NO payload-->
                </div>
                """;
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryBegin, 1, 1));

        // Start CCom section
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER, 13, 1));

        checkExpectToken(lexer, new Token(TokenType.IF, 14, 1));
        checkExpectToken(lexer, new Token(TokenType.HAS, 14, 4));
        checkExpectToken(lexer, new Token(TokenType.NOT, 14, 8));
        checkExpectToken(lexer, new Token(TokenType.TRUE, 14, 12));
        checkExpectToken(lexer, new Token(TokenType.FALSE, 14, 17));
        checkExpectToken(lexer, new Token(TokenType.OR, 14, 23));
        checkExpectToken(lexer, new Token(TokenType.EQUALS, 14, 25));
        checkExpectToken(lexer, new Token(TokenType.NOT_EQUALS, 14, 28));
        checkExpectToken(lexer, new Token(TokenType.LESS, 14, 31));
        checkExpectToken(lexer, new Token(TokenType.LESS_EQUAL, 14, 32));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "36", 14, 35));
        checkExpectToken(lexer, new Token(TokenType.GREATER, 14, 37));
        checkExpectToken(lexer, new Token(TokenType.GREATER_EQUAL, 14, 39));
        checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "testIdentifier", 14, 42));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "42", 14, 57));
        checkExpectToken(lexer, new Token(TokenType.STRING, "TestString", 14, 60));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "73", 14, 73));
        checkExpectToken(lexer, new Token(TokenType.DOT, 14, 75));
        checkExpectToken(lexer, new Token(TokenType.INDEX, "124", 14, 77));

        // Payload
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 14, 82));
        String payload = """
                <div>
                    <p>This is my payload (including div)</p>
                </div>
                {
                }

                """;
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, payload, 15, 1));
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 21, 1));

        // End CCom section
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER, 21, 2));

        // Arbitrary
        String arbitraryMiddle = """


                </body>

                <footer>
                    <p>Test</p>""";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryMiddle, 21, 5));

        // Start CCom section
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER, 26, 16));

        checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "nice", 26, 21));
        checkExpectToken(lexer, new Token(TokenType.STRING, "string", 26, 25));

        // Payload
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 26, 33));
        String payload2 = """
                <div>
                        <p>This is even more payload</>p>
                    </div>""";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, payload2, 26, 34));
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 28, 11));

        // End CCom section
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER, 28, 12));

        // Arbitrary ending
        String arbitraryEnd = """

                </footer>

                </html>
                """;
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryEnd, 28, 15));
    }

    @Test
    @DisplayName("Advanced Java")
    void testAdvancedJava() throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException, UnexpectedTokenException {
        Lexer lexer = constructLexer("AdvancedJavaTest", languages.get("Java"));

        // Arbitrary begin
        String arbitraryBegin = """
                package com.c.lexer;

                class AdvancedJavaTest {

                    private void dummy() {
                        int a = 5;
                        int b = 6;
                        int c = a + b;
                        StringBuilder str = new StringBuilder("asdfjklö ASDFJKLÖ #+-'test' \\"Test\\" [2].3 4<5<6>=1>0 { test } // /* */ }*/");
                       \s""";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryBegin, 1, 1));

        // Start CCom section
        checkExpectToken(lexer, new Token(TokenType.COMMENT_LINE_IDENTIFIER, 10, 9));

        checkExpectToken(lexer, new Token(TokenType.COMMENT_PAYLOAD_IDENTIFIER, 11, 9));
        checkExpectToken(lexer, new Token(TokenType.COMMENT_PAYLOAD_IDENTIFIER, 12, 9));
        checkExpectToken(lexer, new Token(TokenType.COMMENT_PAYLOAD_IDENTIFIER, 13, 9));
        checkExpectToken(lexer, new Token(TokenType.IF, 13, 12));
        checkExpectToken(lexer, new Token(TokenType.HAS, 13, 15));
        checkExpectToken(lexer, new Token(TokenType.NOT, 13, 19));
        checkExpectToken(lexer, new Token(TokenType.TRUE, 13, 23));
        checkExpectToken(lexer, new Token(TokenType.FALSE, 13, 28));
        checkExpectToken(lexer, new Token(TokenType.OR, 13, 34));
        checkExpectToken(lexer, new Token(TokenType.EQUALS, 13, 36));
        checkExpectToken(lexer, new Token(TokenType.NOT_EQUALS, 13, 39));
        checkExpectToken(lexer, new Token(TokenType.LESS, 13, 42));
        checkExpectToken(lexer, new Token(TokenType.LESS_EQUAL, 13, 43));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "36", 13, 46));
        checkExpectToken(lexer, new Token(TokenType.GREATER, 13, 48));
        checkExpectToken(lexer, new Token(TokenType.GREATER_EQUAL, 13, 50));
        checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "testIdentifier", 13, 53));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "42", 13, 68));
        checkExpectToken(lexer, new Token(TokenType.STRING, "TestString", 13, 71));
        checkExpectToken(lexer, new Token(TokenType.NUMBER, "73", 13, 84));
        checkExpectToken(lexer, new Token(TokenType.DOT, 13, 86));
        checkExpectToken(lexer, new Token(TokenType.INDEX, "124", 13, 88));

        // Payload
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 13, 93));
        // note that "//" inside the string got replaced by "" here
        String payload = """
                String strValue = str.toString();
                         strValue += "";
                       \s""";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, payload, 14, 9));

        // Close conditional comment
        checkExpectToken(lexer, new Token(TokenType.COMMENT_LINE_IDENTIFIER, 16, 9));
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 16, 12));

        // Open another conditional comment in this section
        String shortArbitrary = "\n" +
                "        ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, shortArbitrary, 16, 13));
        checkExpectToken(lexer, new Token(TokenType.COMMENT_LINE_IDENTIFIER, 17, 9));

        checkExpectToken(lexer, new Token(TokenType.EQUALS, 17, 13));
        checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "teSt", 17, 16));
        checkExpectToken(lexer, new Token(TokenType.IF, 17, 21));
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 17, 24));

        String arbitraryMiddle = " int d = 5;\n" +
                "        int e = 6;\n" +
                "         int f = d + e;\n" +
                "        ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryMiddle, 18, 9));

        // Close conditional comment
        checkExpectToken(lexer, new Token(TokenType.COMMENT_LINE_IDENTIFIER, 21, 9));
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 21, 12));

        String arbitraryDummy2 = "\n" +
                "    }\n" +
                "\n" +
                "    private void dummy2() {\n" +
                "        ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryDummy2, 21, 13));

        // Start CCom section (now with block comments)
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER, 25, 9));

        checkExpectToken(lexer, new Token(TokenType.STRING, "a", 26, 9));
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 26, 13));

        String payloadDummy2 = "StringBuilder str = new StringBuilder(\"/* { } \");\n" + // "//" replace by ""
                "        ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, payloadDummy2, 27, 9));

        // Close conditional comment
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 28, 9));
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER, 28, 10));

        // Arbitrary between sections
        String arbitraryBetweenSection = "\n" +
                "\n" +
                "        ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryBetweenSection, 28, 12));

        // Start CCom section (now with block comments)
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER, 30, 9));

        checkExpectToken(lexer, new Token(TokenType.STRING, "b", 30, 12));
        checkExpectToken(lexer, new Token(TokenType.NOT_EQUALS, 30, 16));
        checkExpectToken(lexer, new Token(TokenType.LESS, 30, 19));
        checkExpectToken(lexer, new Token(TokenType.IDENTIFIER, "testIdentifier", 30, 21));
        checkExpectToken(lexer, new Token(TokenType.BRACE_OPEN, 30, 36));

        // Payload
        String payloadLast = "StringBuilder str2 = new StringBuilder(\"/* { } \");\n" + // "//" replaced by ""
                "        int z = 42;\n" +
                "           ";
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, payloadLast, 31, 9));

        // Close conditional comment
        checkExpectToken(lexer, new Token(TokenType.BRACE_CLOSE, 33, 12));
        checkExpectToken(lexer, new Token(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER, 33, 13));

        // "//" not replaced (!)
        String arbitraryEnd = """

                    }

                    private String rest() {
                        return "This is the rest // /* */ // { }";
                    }

                }
                """;
        checkExpectToken(lexer, new Token(TokenType.ARBITRARY, arbitraryEnd, 33, 15));
    }


    // ---------------------------------------------- Exceptions -------------------------------------------------------
    // TODO


    // ------------------ Single statements (file only consists of a conditional comments section ----------------------
    // TODO

}
