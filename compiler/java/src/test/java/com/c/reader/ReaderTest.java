package com.c.reader;

import com.c.Constants;
import com.c.TestFileReader;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.IOException;

import static org.assertj.core.api.Assertions.assertThat;

public class ReaderTest {

    private final TestFileReader testFileReader = new TestFileReader("reader");


    // ---------------------------------------- EOF  -------------------------------------------------------------------

    /**
     * Checks that EOF is reached.
     */
    private void expectEOF(Reader reader, int maxLookAhead) throws UnexpectedCharException {
        assertThat(reader.lookAhead()).isEqualTo(Constants.EOF);
        reader.expect(Constants.EOF);

        String bufferExpect = String.valueOf(Constants.EOF).repeat(maxLookAhead);
        assertThat(reader.lookAheads()).isEqualTo(bufferExpect);
    }

    @Test
    @DisplayName("EOF in basic file")
    public void testEof() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 2;
        String file = "Test";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('T');
        reader.advance();
        reader.advance();
        reader.advance();
        assertThat(reader.lookAhead()).isEqualTo('t');
        reader.advance();

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("EOF in empty file")
    public void testEmptyFile() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "";
        Reader reader = new Reader(file, maxLookAhead);

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
        String file = "testfile";

        try {
            new Reader(file, 0);
        } catch (MaxLookAheadException err) {
            assertThat(err.getMessage()).isEqualTo("Max look ahead was 0 but must be at least 1");
        }

        try {
            new Reader(file, -10);
        } catch (MaxLookAheadException err) {
            assertThat(err.getMessage()).isEqualTo("Max look ahead was -10 but must be at least 1");
        }
    }

    @Test
    @DisplayName("Max look ahead of 1 and expect with one character")
    public void testLookAheadMaxOneWithOneChar() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 1;
        String file = "{";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo("{");
        reader.expect('{');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Max look ahead of 2 and expect with one character")
    public void testLookAheadMaxTwoWithOneChar() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 2;
        String file = "{";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo("{" + Constants.EOF);
        reader.expect('{');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Max look ahead of 1 and expect with two character")
    public void testLookAheadMaxOneWithTwoChars() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 1;
        String file = "{a";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo("{");
        reader.expect('{');

        assertThat(reader.lookAhead()).isEqualTo('a');
        assertThat(reader.lookAheads()).isEqualTo("a");
        reader.expect('a');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Max look ahead - line breaks")
    public void testLookAheadMaxLineBreaksWithPosition() throws MaxLookAheadException, UnexpectedCharException {
        // Initialize
        int maxLookAhead = 5;
        String file = "}\n\n\n\n\n\n\n\n";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('}');
        assertThat(reader.lookAheads()).isEqualTo("}" + "\n" + "\n" + "\n" + "\n");
        assertThat(reader.getPosLine()).isEqualTo(1);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('}');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + "\n" + "\n");
        assertThat(reader.getPosLine()).isEqualTo(1);
        assertThat(reader.getPosCol()).isEqualTo(2);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + "\n" + "\n");
        assertThat(reader.getPosLine()).isEqualTo(2);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + "\n" + "\n");
        assertThat(reader.getPosLine()).isEqualTo(3);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + "\n" + "\n");
        assertThat(reader.getPosLine()).isEqualTo(4);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + "\n" + Constants.EOF);
        assertThat(reader.getPosLine()).isEqualTo(5);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + "\n" + Constants.EOF + Constants.EOF);
        assertThat(reader.getPosLine()).isEqualTo(6);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "\n" + Constants.EOF + Constants.EOF + Constants.EOF);
        assertThat(reader.getPosLine()).isEqualTo(7);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + Constants.EOF + Constants.EOF + Constants.EOF + Constants.EOF);
        assertThat(reader.getPosLine()).isEqualTo(8);
        assertThat(reader.getPosCol()).isEqualTo(1);
        reader.expect('\n');
    }


    // --------------------------------------- Look ahead + expect -----------------------------------------------------

    @Test
    @DisplayName("Look ahead and expect with just one simple line break")
    public void testLookAheadWithSimpleLineBreak() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "\n";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with one character")
    public void testLookAheadOneChar() throws MaxLookAheadException, UnexpectedCharException {
        // Initialize
        int maxLookAhead = 4;
        String file = "{";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo("{" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('{');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with two characters")
    public void testLookAheadTwoChars() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "{a";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('{');
        assertThat(reader.lookAheads()).isEqualTo("{" + "a" + Constants.EOF + Constants.EOF);
        reader.expect('{');

        assertThat(reader.lookAhead()).isEqualTo('a');
        assertThat(reader.lookAheads()).isEqualTo("a" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('a');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Expect multiple success")
    public void testExpectMultipleSuccess() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "File input";
        Reader reader = new Reader(file, maxLookAhead);

        reader.advance();
        reader.expectMultiple("ile");
        reader.advance();
        reader.expectMultiple("in");
        reader.advance();
        reader.expectMultiple("ut");

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Expect multiple failure")
    public void testExpectMultipleFailure() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 5;
        String file = "File input";
        Reader reader = new Reader(file, maxLookAhead);

        reader.advance();
        reader.expectMultiple("il");

        try {
            reader.expectMultiple("random");
            throw new Error("Expected reader.expectMultiple() to throw an error, but it didn't");
        } catch (UnexpectedCharException err) {
            assertThat(err.getMessage()).isEqualTo("Expected 'r' but got 'e' @1:4");
        }
    }

    @Test
    @DisplayName("Look ahead and expect with two characters and simple line break")
    public void testLookAheadTwoCharsWithSimpleLineBreak() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "/!\n";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('/');
        assertThat(reader.lookAheads()).isEqualTo("/" + "!" + "\n" + Constants.EOF);
        reader.expect('/');

        assertThat(reader.lookAhead()).isEqualTo('!');
        assertThat(reader.lookAheads()).isEqualTo("!" + "\n" + Constants.EOF + Constants.EOF);
        reader.expect('!');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with two characters and carriage return + new line")
    public void testLookAheadTwoCharsWithCarriageReturnAndNewLine() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "/!\r\n";
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('/');
        assertThat(reader.lookAheads()).isEqualTo("/" + "!" + "\n" + Constants.EOF);
        reader.expect('/');

        assertThat(reader.lookAhead()).isEqualTo('!');
        assertThat(reader.lookAheads()).isEqualTo("!" + "\n" + Constants.EOF + Constants.EOF);
        reader.expect('!');

        // Carriage return is omitted

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }

    @Test
    @DisplayName("Look ahead and expect with multiple lines")
    public void testLookAheadMultipleLines() throws IOException, UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        assertThat(reader.lookAhead()).isEqualTo('a');
        assertThat(reader.lookAheads()).isEqualTo("a" + "b" + "\n" + "c");
        reader.expect('a');

        assertThat(reader.lookAhead()).isEqualTo('b');
        assertThat(reader.lookAheads()).isEqualTo("b" + "\n" + "c" + "d");
        reader.expect('b');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + "c" + "d" + "e");
        reader.expect('\n');

        assertThat(reader.lookAhead()).isEqualTo('c');
        assertThat(reader.lookAheads()).isEqualTo("c" + "d" + "e" + "\n");
        reader.expect('c');

        assertThat(reader.lookAhead()).isEqualTo('d');
        assertThat(reader.lookAheads()).isEqualTo("d" + "e" + "\n" + Constants.EOF);
        reader.expect('d');

        assertThat(reader.lookAhead()).isEqualTo('e');
        assertThat(reader.lookAheads()).isEqualTo("e" + "\n" + Constants.EOF + Constants.EOF);
        reader.expect('e');

        assertThat(reader.lookAhead()).isEqualTo('\n');
        assertThat(reader.lookAheads()).isEqualTo("\n" + Constants.EOF + Constants.EOF + Constants.EOF);
        reader.expect('\n');

        expectEOF(reader, maxLookAhead);
    }


    // ------------------------------- Code position (without caret) ---------------------------------------------------

    @Test
    @DisplayName("Exception msg on unexpected char at the beginning")
    public void testPositionOnUnexpectedCharBeginning() throws IOException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        try {
            reader.expect('z');
            throw new Error("Expected reader.expect() to throw an error, but it didn't");
        } catch (UnexpectedCharException err) {
            assertThat(err.getMessage()).isEqualTo("Expected 'z' but got 'a' @1:1");
        }
    }

    @Test
    @DisplayName("Exception msg on unexpected char in the middle.")
    public void testPositionOnUnexpectedCharMiddle() throws IOException, UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');

        try {
            reader.expect('z');
            throw new Error("Expected reader.expect() to throw an error, but it didn't");
        } catch (UnexpectedCharException err) {
            assertThat(err.getMessage()).isEqualTo("Expected 'z' but got 'd' @2:2");
        }
    }

    @Test
    @DisplayName("Exception msg on unexpected char at the end")
    public void testPositionOnUnexpectedCharEnd() throws IOException, UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');
        reader.expect('d');
        reader.expect('e');

        assertThat(reader.lookAhead()).isNotEqualTo('!');
        try {
            reader.expect('!');
            throw new Error("Expected reader.expect() to throw an error, but it didn't");
        } catch (UnexpectedCharException err) {
            assertThat(err.getMessage()).isEqualTo("Expected '!' but got '\n' @2:4");
        }

    }


    // ------------------------------- Code position (with caret) ------------------------------------------------------

    @Test
    @DisplayName("Position msg with caret with just one simple line break")
    public void testPosMsgWithCaretSimpleLineBreak() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "\n";
        Reader reader = new Reader(file, maxLookAhead);

        String expect1 = "" +
                "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect1);

        reader.expect('\n');
        String expect2 = "" +
                Constants.EOF + "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

    @Test
    @DisplayName("Position msg with caret with one character")
    public void testPosMsgWithCaretOneChar() throws UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = "{";
        Reader reader = new Reader(file, maxLookAhead);

        String expect1 = "" +
                "{\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect1);

        reader.expect('{');
        String expect2 = "" +
                "{" + Constants.EOF + "\n" +
                " ^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

    @Test
    @DisplayName("Position msg with multiples advances after EOF")
    public void testPosMsgWithCaretWithMultiplesAdvancesAfterEOF() throws UnexpectedCharException, IOException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        reader.expect('a');
        reader.expect('b');
        reader.expect('\n');
        reader.expect('c');
        reader.expect('d');
        reader.expect('e');
        reader.expect('\n');
        String expect = "" +
                Constants.EOF + "\n" +
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
    public void testPositionMsgWithCaretBeginning() throws IOException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

        String expect = "" +
                "ab\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect);
    }

    @Test
    @DisplayName("Position msg with caret in the middle.")
    public void testPositionMsgWithCaretMiddle() throws IOException, UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

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
    public void testPositionMsgWithCaretEnd() throws IOException, UnexpectedCharException, MaxLookAheadException {
        // Initialize
        int maxLookAhead = 4;
        String file = testFileReader.fileToString("TwoBasicLines.txt");
        Reader reader = new Reader(file, maxLookAhead);

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
                Constants.EOF + "\n" +
                "^";
        assertThat(reader.toPosStringWithCaret()).isEqualTo(expect2);
    }

}
