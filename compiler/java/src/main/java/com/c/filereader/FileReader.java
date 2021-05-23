package com.c.filereader;

import java.util.PrimitiveIterator;

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
public class FileReader {

    // --------------------------------------- Member variables --------------------------------------------------------

    // Input data as stream
    private final PrimitiveIterator.OfInt file;

    // Buffer
    /**
     * A buffer for the next n characters where n is equal to maxLookAhead (as passed to the constructor)
     */
    private final LimitedQueue<Character> nextChars;

    private final LimitedQueue<String> nextLines;

    // Position in file
    /**
     * The current line we are looking at in the file.
     */
    private int posLine = 1; // first line is line 1

    /**
     * The current column we are looking at in the file.
     */
    private int posCol = 1; // line starts at column 1

    /**
     * The current column we are looking at in the file with the head of our buffer.
     */
    private int posHeadCol = 0; // line starts at column 1

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
     * Constructs a file reader operating on a file given as a string.
     * <p>
     * Note that it would be easy to modify this constructor to be able to pass in a stream instead of a string.
     * The FileReader converts the file that is given as a string (in the current implementation) to a stream anyway.
     *
     * @param file the input file as a string
     */
    public FileReader(String file, int maxLookAhead) {
        this.file = file.chars().iterator();
        this.nextChars = new LimitedQueue<>(maxLookAhead);
        // we take the worst case length here (if every line contained only a single character)
        this.nextLines = new LimitedQueue<>(maxLookAhead);

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
    public Character[] lookAheads() {
        return nextChars.toArray(new Character[0]);
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
            nextChars.add((char) -1);
            return; // nothing to do anymore after having reached EOF
        }

        // HEAD: Need to read next line into buffer?
        if (posHeadCol == nextLines.getLast().length()) {
            readNextLine();
        }

        // Advance
        posHeadCol++;
        nextChars.add(nextLines.getLast().charAt(posHeadCol - 1));
    }

    /**
     * Advances one char with the "normal" position.
     */
    private void advanceNormal() {
        // reached EOF?
        if (hasReachedEof) return;

        // Need to jump to next line with current position?
        if (posCol == nextLines.getFirst().length()) {
            posCol = 0;
            posLine++;
            nextLines.removeFirst(); // advance to the next line
        }

        // Advance with "normal" position and HEAD
        posCol++;

        // Check for EOF for next call to advanceNormal()
        if (nextChars.getFirst().equals((char) -1)) {
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
                newLine.append((char) -1);
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
        posHeadCol = 0;
    }


    // --------------------------------------- Expect  -----------------------------------------------------------------

    /**
     * Checks if the next char is the expected character. Advances one character after a successful check.
     *
     * @param c expected char (to compare the next char against)
     * @throws UnexpectedCharError if the next char is not the expected character
     */
    public void expect(char c) throws UnexpectedCharError {
        if (nextChars.getFirst() != c)
            throw new UnexpectedCharError(c, nextChars.getFirst(), toPosString());
        advance();
    }


    // ------------------------------------ Current position -----------------------------------------------------------

    /**
     * @return formatted line number and column
     */
    private String toPosString() {
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
        String currentLine = nextLines.isEmpty() ? String.valueOf((char) -1) : nextLines.getFirst();

        // Strip last EOF for print out if no EOF yet
        if (!hasReachedEof) {
            if (currentLine.endsWith(String.valueOf((char) -1)))
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

}
