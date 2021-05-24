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
        languages.add(new LanguageDescription(".yml", "//", "/*", "*/"));
        languages.add(new LanguageDescription(".html", "", "<!--", "-->"));
        languages.add(new LanguageDescription(".py", "#", "'''", "'''"));
    }


    // ------------------------------------------- Util ----------------------------------------------------------------

    private interface LanguageCommentCallback {
        void doTestsWithLexer(Lexer lexer, LanguageDescription language) throws UnexpectedCharException, UnknownCharException;
    }

    private void testForEveryLanguage(String filename, LanguageCommentCallback callback)
            throws InvalidCommentsIdentifierException, UnexpectedCharException, MaxLookAheadException, UnknownCharException, IOException {
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

    @Test
    @DisplayName("Test Token EOF in empty file")
    public void testTokenEOFInEmptyFile() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException {
        testForEveryLanguage("Empty", (lexer, language) -> {
            assertThat(lexer.lookAhead()).isEqualTo(new Token(TokenType.EOF, 1, 1));
        });
    }

    @Test
    @DisplayName("Test Token IF")
    public void testTokenIf() throws MaxLookAheadException, InvalidCommentsIdentifierException, UnexpectedCharException, UnknownCharException, IOException {
        testForEveryLanguage("If", (lexer, language) -> {
            lexer.advance(); // Consume comment open, e.g. "//?"
            assertThat(lexer.lookAhead()).isEqualTo(new Token(TokenType.IF, 1, 1));
        });
    }


    // ------------------ Single statements (file only consists of a conditional comments section ----------------------


}
