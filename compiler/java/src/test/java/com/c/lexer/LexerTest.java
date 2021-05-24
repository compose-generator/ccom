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

    private Token constructExpectedTokenIndividualTokenTestStart(LanguageDescription language) {
        int column;
        TokenType type;
        if (!language.getCommentLineIdentifier().isEmpty()) { // line comment
            column = language.getCommentLineIdentifier().length();
            type = TokenType.COMMENT_LINE_IDENTIFIER;
        } else { // block comment
            column = language.getCommentBlockOpenIdentifier().length();
            type = TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER;
        }
        column++; // for ? after actual comment
        column++; // for space

        return new Token(type, 1, column);
    }

    private Token checkCommentStart(Lexer lexer, LanguageDescription language) throws UnexpectedTokenException, UnexpectedCharException, UnknownCharException {
        Token expectedStartToken = constructExpectedTokenIndividualTokenTestStart(language);
        assertThat(lexer.lookAhead()).isEqualTo(expectedStartToken);
        lexer.expect(expectedStartToken);

        return expectedStartToken;
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
            Token expectedStartToken = checkCommentStart(lexer, language);

            // +1 for space
            // +2 for "if"
            Token expectedIfToken = new Token(TokenType.IF, 1, expectedStartToken.getPosCol() + 1 + 2);
            assertThat(lexer.lookAhead()).isEqualTo(expectedIfToken);
            lexer.expect(expectedIfToken);
        });
    }

    @Test
    @DisplayName("Test Token HAS")
    public void testTokenHas() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Has", (lexer, language) -> {
            Token expectedStartToken = checkCommentStart(lexer, language);

            // +1 for space
            // +3 for "has"
            Token expectedHasToken = new Token(TokenType.HAS, 1, expectedStartToken.getPosCol() + 1 + 3);
            assertThat(lexer.lookAhead()).isEqualTo(expectedHasToken);
            lexer.expect(expectedHasToken);
        });
    }

    @Test
    @DisplayName("Test Token NOT")
    public void testTokenNot() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("Not", (lexer, language) -> {
            Token expectedStartToken = checkCommentStart(lexer, language);

            // +1 for space
            // +3 for "not"
            Token expectedNotToken = new Token(TokenType.NOT, 1, expectedStartToken.getPosCol() + 1 + 3);
            assertThat(lexer.lookAhead()).isEqualTo(expectedNotToken);
            lexer.expect(expectedNotToken);
        });
    }

    @Test
    @DisplayName("Test Token TRUE")
    public void testTokenTrue() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("True", (lexer, language) -> {
            Token expectedStartToken = checkCommentStart(lexer, language);

            // +1 for space
            // +4 for "true"
            Token expectedTrueToken = new Token(TokenType.TRUE, 1, expectedStartToken.getPosCol() + 1 + 4);
            assertThat(lexer.lookAhead()).isEqualTo(expectedTrueToken);
            lexer.expect(expectedTrueToken);
        });
    }

    @Test
    @DisplayName("Test Token False")
    public void testTokenFalse() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException, UnexpectedTokenException {
        testForEveryLanguage("False", (lexer, language) -> {
            Token expectedStartToken = checkCommentStart(lexer, language);

            // +1 for space
            // +5 for "false"
            Token expectedFalseToken = new Token(TokenType.FALSE, 1, expectedStartToken.getPosCol() + 1 + 5);
            assertThat(lexer.lookAhead()).isEqualTo(expectedFalseToken);
            lexer.expect(expectedFalseToken);
        });
    }


    // ------------------ Single statements (file only consists of a conditional comments section ----------------------


}
