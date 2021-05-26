package com.c.lexer;

import com.c.LanguageDescription;
import com.c.TestFileReader;
import com.c.filereader.MaxLookAheadException;
import com.c.filereader.UnexpectedCharException;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;

import static org.assertj.core.api.Assertions.assertThat;

public class LexerTest {

    private final TestFileReader testFileReader = new TestFileReader("lexer");
    private static final ArrayList<LanguageDescription> languages = new ArrayList<>();

    public LexerTest() {
        // Comment identifiers for different languages
        languages.add(new LanguageDescription(".java", "//", "/*", "*/"));
        languages.add(new LanguageDescription(".yml", "#", "", ""));
        languages.add(new LanguageDescription(".html", "", "<!--", "-->"));
        languages.add(new LanguageDescription(".py", "#", "'''", "'''"));
    }


    // ------------------------------------------- Util ----------------------------------------------------------------

    private interface LanguageCommentCallback {
        void doTestsWithLexer(Lexer lexer, LanguageDescription language) throws UnexpectedCharException, UnknownCharException, UnexpectedTokenException;
    }

    private void testForEveryLanguage(String filename, LanguageCommentCallback callback)
            throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException, UnexpectedTokenException {
        for (LanguageDescription language : languages) {
            String file = testFileReader.fileToString(filename + language.getFileExtension());
            Lexer lexer = new Lexer(
                    file,
                    language.getCommentLineIdentifier(),
                    language.getCommentBlockOpenIdentifier(),
                    language.getCommentBlockCloseIdentifier(),
                    false
            );
            callback.doTestsWithLexer(lexer, language);
        }
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


    // -- Different Context
    // TODO


    // -- Test all exception
    // TODO


    // ------------------ Single statements (file only consists of a conditional comments section ----------------------
    // TODO

}
