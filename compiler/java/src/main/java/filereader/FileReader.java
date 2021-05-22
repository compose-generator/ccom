package filereader;

import java.util.PrimitiveIterator;

/**
 * FileReader that is capable of looking ahead one char.
 * Only operates on strings, not actual files.
 */
public class FileReader {

    // Input data as stream
    private final PrimitiveIterator.OfInt file;

    // Buffer
    /**
     * A buffer for the next n characters where n is equal to maxLookAhead (as passed to the constructor)
     */
    private final LimitedQueue<Character> nextChars;
    /**
     * The current buffered line of the input file
     */
    private String currentLine = "";

    // Current position in code (row and column)
    /**
     * The current line we are looking at (first element of buffer) in the file.
     */
    private int posRow;
    /**
     * The current column we are looking at (first element of buffer) in the current line of the file.
     */
    private int posCol;


    /**
     * Constructs a file reader operating on a file given as a string.
     *
     * @param file the input file as a string
     */
    public FileReader(String file, int maxLookAhead) {
        this.file = file.chars().iterator();
        this.nextChars = new LimitedQueue<>(maxLookAhead);

        // Advance to load first characters into buffer
        for (int i = 0; i < maxLookAhead; i++) {
            advance();
        }

        // Keep position in sync with first element of buffer
        posRow = 0;
        posCol = 0;
    }

    /**
     * Looks ahead one char without consuming it.
     * -1 stands for EOF.
     *
     * @return next char
     */
    public char lookAhead() {
        return nextChars.get(0);
    }

    /**
     * Looks ahead as many chars (as given by maxLookAhead passed to the FileReader) without consuming them.
     * -1 stands for EOF.
     *
     * @return next chars as a character list
     */
    public Character[] lookAheads() {
        return nextChars.toArray(new Character[0]);
    }

    /**
     * Consumes current char and advances to next character.
     */
    private void advance() {
        // Check for EOF
        if (!file.hasNext()) {
            nextChars.add((char) -1);
            return; // nothing to do anymore after having reached EOF
        }

        // Check if we need to read the next line
        if (posCol == currentLine.length() - 1) {
            readNextLine(); // will refresh line buffer
            posCol = 0; // reset column
            posRow = 0; // reset row
        }

        // Add to next chars buffer
        nextChars.add((char) file.nextInt());
        posRow++;
    }

    /**
     * Reads next line and stores it in the line buffer.
     * Note that this will convert '\r\n' line breaks to a simple '\n'.
     */
    private void readNextLine() {
        StringBuilder line = new StringBuilder();

        lineReader:
        while (true) {
            // Check for EOF
            if (!file.hasNext()) {
                currentLine = "";
                return;
            }
            int nextChar = file.next();
            switch (nextChar) {
                case '\r':  // carriage return
                    // convert '\r\n' line breaks to a simple '\n'
                    continue;
                case '\n': // new line
                    line.append('\n');
                    break lineReader;
                default:
                    line.append(nextChar);
            }
        }

        currentLine = line.toString();
    }

    /**
     * Checks if the next char is the expected character. Advances one character after a successful check.
     *
     * @param c expected char (to compare the next char against)
     * @throws UnexpectedCharError if the next char is not the expected character
     */
    public void expect(char c) throws UnexpectedCharError {
        if (nextChars.get(0) != c)
            throw new UnexpectedCharError(c, nextChars.get(0), locationToLineAndCol());
        advance();
    }

    /**
     * Output example:<br>
     * public void test() {...}<br>
     * ............... ^
     *
     * @return the current line with a caret at the current position
     */
    public String getCaretPositionMsg() {
        return currentLine + " ".repeat(posCol) + "^\n";
    }

    /**
     * @return formatted line number and column
     */
    private String locationToLineAndCol() {
        return "@L" + posRow + "C" + posCol;
    }

}
