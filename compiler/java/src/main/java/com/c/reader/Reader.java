package com.c.reader;

import com.c.Constants;

import java.util.ArrayList;
import java.util.PrimitiveIterator;
import java.util.stream.Collectors;

/**
 * FileReader that is capable of looking ahead one char.
 * Only operates on strings, not actual files.<br><br>
 * <p>
 * This file reader might have a look ahead great than 1. This is why we keep track of two positions:
 * - One position is prefixed with HEAD. Characters left to this position are all read in in some buffer.
 * Characters right to this position will be read in with the next calls to advance().
 * - The other position is not prefixed. It is the position that a user of this FileReader would assume
 * using the formatting methods like getCaretPositionMsg().
 * <p>
 * The difference between those two can be seen right at the start. Assume that maxLookAhead is 4.
 * We need to fill up the buffer, so the HEAD position is pointing to character 4 while the "normal" position still
 * points to 0 as a user of this FileReader wants to start with the first character (and not the fifth).
 */
public class Reader {

    // --------------------------------------- Member variables --------------------------------------------------------

    // File

    /**
     * The input file as a char stream.
     */
    private final PrimitiveIterator.OfInt file;


    // Buffer

    /**
     * A buffer for the next n characters where n is equal to maxLookAhead (as passed to the constructor)
     */
    private final LimitedQueue<Character> nextChars;

    /**
     * A buffer for the next n lines where n is at least equal to maxLookAhead (as passed to the constructor).
     * <p>
     * "At least" since there might be lots of new lines read in by the HEAD while we are still on one line
     * with the "normal" position.
     */
    private final ArrayList<String> nextLines;


    // Position in file

    /**
     * The current line we are looking at in the file. Counted from 1 onwards.
     */
    private int posLine = 1; // first line is line 1

    /**
     * The current column we are looking at in the file. Counted from 1 onwards.
     */
    private int posCol = 1; // line starts at column 1

    /**
     * The current column we are looking at in the file with the head of our buffer. Counted from 1 onwards.
     */
    private int posHeadCol = 1; // line starts at column 1


    // EOF flag

    /**
     * Flag for whether the HEAD has reached EOF (end of file) or not.
     */
    private boolean hasHeadReachedEof = false;

    /**
     * Flag for whether the "normal" position has reached EOF (end of file) or not.
     */
    private boolean hasReachedEof = false;


    // --------------------------------------- Constructor -------------------------------------------------------------

    /**
     * Constructs a FileReader operating on a file given as String.
     * <p>
     * Note that it would be easy to modify this constructor to be able to pass in a stream instead of a string.
     * The FileReader converts the file that is given as a string (in the current implementation) to a stream anyway.
     *
     * @param file the input file as String
     * @throws MaxLookAheadException if the max look ahead is less than 1
     */
    public Reader(String file, int maxLookAhead) throws MaxLookAheadException {
        if (maxLookAhead < 1)
            throw new MaxLookAheadException(maxLookAhead);
        this.file = file.chars().iterator();

        this.nextChars = new LimitedQueue<>(maxLookAhead);
        this.nextLines = new ArrayList<>();

        // Advance to load first characters into buffer
        readNextLine();
        // even if EOF is reached when having read just one line
        // we need to add the next chars in the advance() call
        // afterwards: hasReachedEof is set with the next readNextLine() call
        hasHeadReachedEof = false;
        for (int i = 0; i < maxLookAhead; i++) {
            advanceHead();
        }
    }


    // ----------------------------------------- Look ahead ------------------------------------------------------------

    /**
     * Looks ahead one char without consuming it.
     * -1 stands for EOF.
     *
     * @return next char
     */
    public char lookAhead() {
        return nextChars.getFirst();
    }

    /**
     * Looks ahead the next n chars where n is equal to maxLookAHead (as passed to the constructor)
     * -1 stands for EOF.
     *
     * @return next chars as a character list
     */
    public String lookAheads() {
        return nextChars.stream().map(Object::toString).collect(Collectors.joining());
    }


    // ----------------------------------------- Advance ---------------------------------------------------------------

    /**
     * Consumes current char and advances to next character.
     */
    public void advance() {
        advanceHead();
        advanceNormal();
    }

    /**
     * Advances one char with the HEAD position.
     */
    private void advanceHead() {
        // Check for EOF
        if (hasHeadReachedEof) {
            nextChars.add(Constants.EOF);
            return; // nothing to do anymore after having reached EOF
        }

        // we explicitly don't check if nextLines is empty here as it should never be (!)
        // that's why we make a call to readNextLine() in the constructor before advanceHead()
        String lastLine = nextLines.get(nextLines.size() - 1);

        // HEAD: Need to read next line into buffer?
        if (posHeadCol == lastLine.length() + 1) {
            readNextLine();
            lastLine = nextLines.get(nextLines.size() - 1);
        }

        // Advance
        nextChars.add(lastLine.charAt(posHeadCol - 1));
        posHeadCol++;
    }

    /**
     * Advances one char with the "normal" position.
     */
    private void advanceNormal() {
        // reached EOF?
        if (hasReachedEof) return;

        // Need to jump to next line with current position?
        if (posCol == nextLines.get(0).length()) {
            posCol = 0;
            posLine++;
            nextLines.remove(0); // advance to the next line
        }

        // Advance with "normal" position
        posCol++;

        // Check for EOF for next call to advanceNormal()
        if (nextChars.getFirst().equals(Constants.EOF)) {
            hasReachedEof = true;
        }
    }

    /**
     * Reads next line and stores it in the line buffer. Operating with the HEAD position.
     * Note that this will convert '\r\n' line breaks to a simple '\n'.
     */
    private void readNextLine() {
        StringBuilder newLine = new StringBuilder();

        lineReader:
        while (true) {

            // Check for EOF
            if (!file.hasNext()) {
                hasHeadReachedEof = true;
                // construct a dummy currentLine for advance() to read from once at the end
                newLine.append(Constants.EOF);
                break;
            }

            char next = (char) file.nextInt();
            switch (next) {
                case '\r':  // carriage return
                    // convert '\r\n' line breaks to a simple '\n'
                    continue;
                case '\n': // new line
                    newLine.append('\n');
                    break lineReader;
                default: // every other char
                    newLine.append(next);
            }

        }

        nextLines.add(newLine.toString());
        posHeadCol = 1; // line start with col 1
    }


    // --------------------------------------- Expect ------------------------------------------------------------------

    /**
     * Checks if the next char is the expected character. Advances one character after a successful check.
     *
     * @param c expected char (to compare the next char against)
     * @throws UnexpectedCharException if the next char is not the expected character
     */
    public void expect(char c) throws UnexpectedCharException {
        if (nextChars.getFirst() != c)
            throw new UnexpectedCharException(c, nextChars.getFirst(), toPosString());
        advance();
    }

    /**
     * Checks if the next chars are equal to the given String. Advances n character after a successful check
     * where n is the length of the String.
     *
     * @param expected expected chars as string (to compare the next chars against)
     * @throws UnexpectedCharException if one of the next chars is not the expected character (in the given String)
     */
    public void expectMultiple(String expected) throws UnexpectedCharException {
        for (char c : expected.toCharArray()) {
            expect(c);
        }
    }


    // ------------------------------------ Current position -----------------------------------------------------------

    /**
     * @return formatted line number and column
     */
    public String toPosString() {
        return "@" + posLine + ":" + posCol;
    }

    /**
     * Output example:<br>
     * public void test() {...}<br>
     * ............... ^
     *
     * @return the current line with a caret at the current position
     */
    public String toPosStringWithCaret() {
        String currentLine = nextLines.isEmpty() ? String.valueOf(Constants.EOF) : nextLines.get(0);

        // Strip last EOF for print out if no EOF yet
        if (!hasReachedEof) {
            if (currentLine.endsWith(String.valueOf(Constants.EOF)))
                currentLine = currentLine.substring(0, currentLine.length() - 1);
        }

        StringBuilder posMsg = new StringBuilder(currentLine);

        // Add \n at the end if not already there
        if (!currentLine.endsWith("\n"))
            posMsg.append('\n');

        posMsg.append(" ".repeat(posCol - 1));
        posMsg.append("^");

        return posMsg.toString();
    }

    public int getPosLine() {
        return posLine;
    }

    public int getPosCol() {
        return posCol;
    }

}
