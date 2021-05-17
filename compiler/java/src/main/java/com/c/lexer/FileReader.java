package com.c.lexer;

/**
 * FileReader that is capable of looking ahead one char.
 * Only operates on strings, not actual files.
 */
public class FileReader {
    private String file;
    private char next; // Buffer

    // Current position
    private int lineNum;
    private int lineCol;

    // Current line
    private String currentLine;

    private boolean isLastLine;


    public FileReader(String file) {
        this.file = file;
    }

    public char lookAheadChar() {
        return next;
    }

    public void expect(char c) throws Exception {
        if (next != c)
            throw new Error("Expected '" + c + "', but got '" + next + "' " + locationToLineAndCol());
        advance();
    }

    private void advance() throws Exception {
//        while (currentLinePos == currentLine.length()) {
//            if (isLastLine) {
//                next = 0;
//                return;
//            } else {
//                readNextLine();
//                currentLinePos = 0;
//            }
//        }
//
//        InputStringPos++;
//        if (InputStringPos > FileInput.length() - 1) {
//            CurrentChar = EOF; // Return EOF, when string ends
//        } else {
//            CurrentChar = (unsigned char)FileInput[InputStringPos];
//            ColNum++;
//        }
//        return CurrentChar;
//
//        while (m_currentLineNextPos == m_currentLine.length()) {
//            if (m_eofAfterCurrentLine) {
//                m_nextChar = 0;
//                return;
//            } else {
//                readNextLine();
//                m_currentLineNextPos = 0;
//            }
//        }
//        m_nextChar = m_currentLine.charAt(m_currentLineNextPos);
//        m_currentLineNextPos++;
    }

    /**
     * Output example:<br>
     * public void test() {...}<br>
     * ^
     *
     * @return the current line with a caret at the current position
     */
    public String locationToCaret() {
        StringBuilder location = new StringBuilder(currentLine);
        location.append(" ".repeat(Math.max(0, lineCol)));
        location.append("^\n");
        return location.toString();
    }

    private String locationToLineAndCol() {
        return "@L" + lineNum + "C" + lineCol;
    }

}
