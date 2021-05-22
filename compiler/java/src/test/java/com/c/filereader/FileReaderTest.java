package com.c.filereader;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.Arrays;

import static com.c.testfiles.FileToString.fileToString;
import static org.assertj.core.api.Assertions.assertThat;

public class FileReaderTest {

    // ---------------------------------------- Util -------------------------------------------------------------------

    /**
     * Expects to read EOF.
     */
    private void expectEOF(FileReader reader, int maxLookAhead) throws UnexpectedCharError {
        assertThat(reader.lookAhead()).isEqualTo((char) -1);
        reader.expect((char) -1);

        Character[] bufferExpect = new Character[maxLookAhead];
        Arrays.fill(bufferExpect, (char) -1);
        assertThat(reader.lookAheads()).isEqualTo(bufferExpect);
    }


    // ---------------------------------------- EOF --------------------------------------------------------------------
    @Test
    @DisplayName("EOF in empty file")
    public void testEmptyFile() throws UnexpectedCharError {
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


    // --------------------------------------- Look ahead + expect -----------------------------------------------------

    @Test
    @DisplayName("Look ahead and expect with multiple lines")
    public void testLookAheadMultipleLines() throws IOException, UnexpectedCharError {
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

//    // ------------------------------------- Code position -------------------------------------------------------------
//    @Test
//    @DisplayName("Exception msg on unexpected char at the beginning")
//    public void testPositionOnUnexpectedChar1() throws IOException {
//        // Initialize
//        int maxLookAhead = 4;
//        String file = fileToString("TwoBasicLines.txt");
//        FileReader reader = new FileReader(file, maxLookAhead);
//
//        try {
//            reader.expect('z');
//        } catch (UnexpectedCharError err) {
//            assertThat(err.getMessage()).isEqualTo("Expected 'z' but got 'a' @1:1");
//        }
//
//        throw new Error("Expected reader.expect() to throw an error, but it didn't");
//    }

}
