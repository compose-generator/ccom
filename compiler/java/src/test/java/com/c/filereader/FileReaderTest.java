package com.c.filereader;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Arrays;

import static com.c.testfiles.FileToString.fileToString;
import static org.assertj.core.api.Assertions.assertThat;

public class FileReaderTest {

    // ---------------------------------------- EOF  -------------------------------------------------------------------

    /**
     * Checks that EOF is reached.
     */
    private void expectEOF(FileReader reader, int maxLookAhead) throws UnexpectedCharError {
        assertThat(reader.lookAhead()).isEqualTo((char) -1);
        reader.expect((char) -1);

        Character[] bufferExpect = new Character[maxLookAhead];
        Arrays.fill(bufferExpect, (char) -1);
        assertThat(reader.lookAheads()).isEqualTo(bufferExpect);
    }

    @Test
    @DisplayName("EOF in empty file")
    public void testEmptyFile() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "";
        FileReader reader = new FileReader(file, maxLookAhead);

        expectEOF(reader, maxLookAhead);

        // Advance and still be at EOF
        reader.advance();
        reader.advance();
        reader.advance();
        reader.advance();
        reader.advance();

        expectEOF(reader, maxLookAhead);
    }

    // --------------------------------------- Max look ahead ----------------------------------------------------------
    @Test
    @DisplayName("Max look ahead is less than 1")
    public void testMaxLookAheadLessThan1() {
        // Initialize
        int maxLookAhead = 0;
        String file = "\n";

        try {
            new FileReader(file, maxLookAhead);
        } catch (MaxLookAheadError err) {
            assertThat(err.getMessage()).isEqualTo("Max look ahead was 0 but must be greater than 0");
        }
    }


    // --------------------------------------- Look ahead + expect -----------------------------------------------------

    @Test
    @DisplayName("Look ahead and expect with just one simple line break")
    public void testLookAheadWithSimpleLineBreak() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "\n";
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'\n', (char) -1, (char) -1, (char) -1});
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with one character")
    public void testLookAheadOneChar() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "{";
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'{', (char) -1, (char) -1, (char) -1});
        reader.expect('{');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with two characters")
    public void testLookAheadTwoChars() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "{a";
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'{', 'a', (char) -1, (char) -1});
        reader.expect('{');

        assertThat(reader.lookAhead()).isEqualTo('a');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'a', (char) -1, (char) -1, (char) -1});
        reader.expect('a');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with two characters and simple line break")
    public void testLookAheadTwoCharsWithSimpleLineBreak() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "/!\n";
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('/');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'/', '!', '\n', (char) -1});
        reader.expect('/');

        assertThat(reader.lookAhead()).isEqualTo('!');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'!', '\n', (char) -1, (char) -1});
        reader.expect('!');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'\n', (char) -1, (char) -1, (char) -1});
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with two characters and carriage return + new line")
    public void testLookAheadTwoCharsWithCarriageReturnAndNewLine() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "/!\r\n";
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('/');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'/', '!', '\n', (char) -1});
        reader.expect('/');

        assertThat(reader.lookAhead()).isEqualTo('!');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'!', '\n', (char) -1, (char) -1});
        reader.expect('!');

        // Carriage return is omitted

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'\n', (char) -1, (char) -1, (char) -1});
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and epxect with multiple lines")
    public void testLookAheadMultipleLines() throws IOException, UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('a');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'a', 'b', '\n', 'c'});
        reader.expect('a');

        assertThat(reader.lookAhead()).isEqualTo('b');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'b', '\n', 'c', 'd'});
        reader.expect('b');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'\n', 'c', 'd', 'e'});
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('c');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'c', 'd', 'e', '\n'});
        reader.expect('c');

        assertThat(reader.lookAhead()).isEqualTo('d');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'d', 'e', '\n', (char) -1});
        reader.expect('d');

        assertThat(reader.lookAhead()).isEqualTo('e');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'e', '\n', (char) -1, (char) -1});
        reader.expect('e');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo(new Character[]{'\n', (char) -1, (char) -1, (char) -1});
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }


    // ------------------------------- Code position (without caret) ---------------------------------------------------

    @Test
    @DisplayName("Exception msg on unexpected char at the beginning")
    public void testPositionOnUnexpectedCharBeginning() throws IOException, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        try {
            reader.expect('z');
        } catch (UnexpectedCharError err) {
            assertThat(err.getMessage()).isEqualTo("Expected 'z' but got 'a' @1:1");
            return;
        }

        throw new Error("Expected reader.expect() to throw an error, but it didn't");
    }

    @Test
    @DisplayName("Exception msg on unexpected char in the middle.")
    public void testPositionOnUnexpectedCharMiddle() throws IOException, UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');

        try {
            reader.expect('z');
        } catch (UnexpectedCharError err) {
            assertThat(err.getMessage()).isEqualTo("Expected 'z' but got 'd' @2:2");
            return;
        }

        throw new Error("Expected reader.expect() to throw an error, but it didn't");
    }

    @Test
    @DisplayName("Exception msg on unexpected char at the end")
    public void testPositionOnUnexpectedCharEnd() throws IOException, UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');
        reader.expect('d');
        reader.expect('e');

        assertThat(reader.lookAhead()).isNotEqualTo('!');
        try {
            reader.expect('!');
        } catch (UnexpectedCharError err) {
            assertThat(err.getMessage()).isEqualTo("Expected '!' but got '\n' @2:4");
            return;
        }

        throw new Error("Expected reader.expect() to throw an error, but it didn't");
    }


    // ------------------------------- Code position (with caret) ------------------------------------------------------

    @Test
    @DisplayName("Position msg with caret with just one simple line break")
    public void testPosMsgWithCaretSimpleLineBreak() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "\n";
        FileReader reader = new FileReader(file, maxLookAhead);

        String expect1 = "" +
                "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect1);

        reader.expect('\n');
        String expect2 = "" +
                (char) -1 + "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

    @Test
    @DisplayName("Position msg with caret with one character")
    public void testPosMsgWithCaretOneChar() throws UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = "{";
        FileReader reader = new FileReader(file, maxLookAhead);

        String expect1 = "" +
                "{\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect1);

        reader.expect('{');
        String expect2 = "" +
                "{" + (char) -1 + "\n" +
                " ^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

    @Test
    @DisplayName("Position msg with multiples advances after EOF")
    public void testPosMsgWithCaretWithMultiplesAdvancesAfterEOF() throws UnexpectedCharError, IOException, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');
        reader.expect('d');
        reader.expect('e');
        reader.expect('\n');
        String expect = "" +
                (char) -1 + "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);

        reader.advance();
        reader.advance();
        reader.advance();
        reader.advance();
        reader.advance();
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);

        reader.advance();
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);
    }

    @Test
    @DisplayName("Position msg with caret at the beginning")
    public void testPositionMsgWithCaretBeginning() throws IOException, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        String expect = "" +
                "ab\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);
    }

    @Test
    @DisplayName("Position msg with caret in the middle.")
    public void testPositionMsgWithCaretMiddle() throws IOException, UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');

        String expect = "" +
                "cde\n" +
                " ^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);
    }

    @Test
    @DisplayName("Position msg with caret at the end")
    public void testPositionMsgWithCaretEnd() throws IOException, UnexpectedCharError, MaxLookAheadError {
        // Initialize
        int maxLookAhead = 4;
        String file = fileToString("TwoBasicLines.txt");
        FileReader reader = new FileReader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');
        reader.expect('d');
        reader.expect('e');
        String expect1 = "" +
                "cde\n" +
                "   ^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect1);

        reader.expect('\n');
        String expect2 = "" +
                (char) -1 + "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

}
