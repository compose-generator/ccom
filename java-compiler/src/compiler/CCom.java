package compiler;

public class CCom implements CComIntf {

    private static final String COM_LINE_IDEN = "//?"; // must be 3 characters long
    private static final String COM_BLOCK_IDEN_OPEN = "/*?"; // must be 3 characters long
    private static final String COM_BLOCK_IDEN_CLOSE = "*/"; // must be 2 characters long
    private static final String COM_IDEN_PAYLOAD = "//??"; // must be 4 characters long, must be an extension to
                                                           // COM_LINE_IDEN

    private FileReaderIntf reader;

    public CCom(FileReaderIntf fileReader) {
        this.reader = fileReader;
    }
    // CONTENT --> (CHARS SECTION)* CHARS
    // SECTION --> COM_LINE_BLOCK* COM_BLOCK_BLOCK*

    @Override
    public String getStart() throws Exception {
        return getComLineBlock();
    }

    /**
     * COM_LINE_BLOCK --> COM_LINE_IDEN if STMT_LST COM_LINE_IDEN? { PAYLOAD
     * COM_LINE_IDEN }
     */
    public String getComLineBlock() throws Exception {
        StringBuilder sb = new StringBuilder();

        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[0]));
        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[1]));
        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[2]));

        sb.append(reader.expect('i'));
        sb.append(reader.expect('f'));
        sb.append(getStatementList());

        if (reader.lookAheadChar() != '{') {
            sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[0]));
            sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[1]));
            sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[2]));
        }
        sb.append(reader.expect('{'));

        sb.append(getPayload());

        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[0]));
        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[1]));
        sb.append(reader.expect(COM_LINE_IDEN.toCharArray()[2]));
        sb.append(reader.expect('}'));

        return sb.toString();
    }

    /**
     * COM_BLOCK_BLOCK --> COM_BLOCK_IDEN_OPEN IF_BLOCK* COM_BLOCK_IDEN_CLOSE
     */
    public String getComBlockBlock() throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append(reader.expect(COM_BLOCK_IDEN_OPEN.toCharArray()[0]));
        sb.append(reader.expect(COM_BLOCK_IDEN_OPEN.toCharArray()[1]));
        sb.append(reader.expect(COM_BLOCK_IDEN_OPEN.toCharArray()[2]));

        while (reader.lookAheadChar() != COM_BLOCK_IDEN_CLOSE.toCharArray()[0]) {
            sb.append(getIfBlock());
        }

        sb.append(reader.expect(COM_BLOCK_IDEN_CLOSE.toCharArray()[0]));
        sb.append(reader.expect(COM_BLOCK_IDEN_CLOSE.toCharArray()[1]));

        return sb.toString();
    }

    /**
     * IF_BLOCK --> if STMT_LST { PAYLOAD }
     */
    public String getIfBlock() throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append(reader.expect('i'));
        sb.append(reader.expect('f'));
        sb.append(getStatementList());
        sb.append(reader.expect('{'));
        sb.append(getPayload());
        sb.append(reader.expect('}'));
        return sb.toString();
    }

    /**
     * PAYLOAD --> (COM_IDEN_PAYLOAD CHARS)+
     */
    public String getPayload() throws Exception {
        StringBuilder sb = new StringBuilder();
        do {
            if (reader.lookAheadChar4().toCharArray()[3] != COM_IDEN_PAYLOAD.toCharArray()[3])
                break; // we encountered //! instead of //!!
            sb.append(reader.expect(COM_IDEN_PAYLOAD.toCharArray()[0]));
            sb.append(reader.expect(COM_IDEN_PAYLOAD.toCharArray()[1]));
            sb.append(reader.expect(COM_IDEN_PAYLOAD.toCharArray()[2]));
            sb.append(reader.expect(COM_IDEN_PAYLOAD.toCharArray()[3]));
            sb.append(getChars());
        } while (isFollowChars(reader.lookAheadChar()));
        return sb.toString();
    }

    /**
     * STMT_LST --> STMT (`|` STMT)*
     */
    public String getStatementList() throws Exception {
        StringBuilder sb = new StringBuilder();
        char next = reader.lookAheadChar();
        sb.append(getStatement());
        next = reader.lookAheadChar();
        while (!isFollowStatement(next)) {
            sb.append(reader.expect('|'));
            sb.append(getStatement());
            next = reader.lookAheadChar();
        }
        return sb.toString();
    }

    /**
     * STMT --> HAS_STMT | COMP_STMT
     */
    public String getStatement() throws Exception {
        String lookAhead3 = reader.lookAheadChar3();
        if (lookAhead3.equals("not") || lookAhead3.equals("has")) {
            return getHasStatement();
        } else {
            return getCompareStatement();
        }
    }

    /**
     * HAS_STMT --> has KEY | not has KEY
     */
    public String getHasStatement() throws Exception {
        StringBuilder sb = new StringBuilder();
        if (reader.lookAheadChar() == 'n') {
            sb.append(reader.expect('n'));
            sb.append(reader.expect('o'));
            sb.append(reader.expect('t'));
            sb.append(reader.expect('h'));
            sb.append(reader.expect('a'));
            sb.append(reader.expect('s'));
        } else {
            sb.append(reader.expect('h'));
            sb.append(reader.expect('a'));
            sb.append(reader.expect('s'));
        }
        sb.append(getKey());
        return sb.toString();
    }

    /**
     * COMP_STMT --> KEY == VALUE | KEY != VALUE
     */
    public String getCompareStatement() throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append(getKey());
        if (reader.lookAheadChar() == '=') {
            sb.append(reader.expect('='));
            sb.append(reader.expect('='));
        } else {
            sb.append(reader.expect('!'));
            sb.append(reader.expect('='));
        }
        sb.append(getValue());
        return sb.toString();
    }

    /**
     * KEY --> IDENTIFIER(.IDENTIFIER)*
     */
    public String getKey() throws Exception {
        StringBuilder sb = new StringBuilder();
        char next = reader.lookAheadChar();
        sb.append(getIdentifier());
        next = reader.lookAheadChar();
        while (!isFollowIdentifier(next)) {
            sb.append(reader.expect('.'));
            sb.append(getIdentifier());
            next = reader.lookAheadChar();
        }
        return sb.toString();
    }

    /**
     * IDENTIFIER --> LETTER+
     */
    public String getIdentifier() throws Exception {
        StringBuilder sb = new StringBuilder();
        do {
            sb.append(getLetter());
        } while (Character.isLetter(reader.lookAheadChar()));
        return sb.toString();
    }

    /**
     * VALUE --> STRING | NUMBER
     */
    public String getValue() throws Exception {
        if (Character.isDigit(reader.lookAheadChar())) {
            return getNumber();
        } else {
            return getString();
        }
    }

    /**
     * STRING --> "CHARS_LIMITED"
     */
    public String getString() throws Exception {
        reader.expect('"');
        String result = getCharsLimited();
        reader.expect('"');
        return '"' + result + '"';
    }

    /**
     * NUMBER --> DIGIT+
     */
    public String getNumber() throws Exception {
        StringBuilder sb = new StringBuilder();
        do {
            sb.append(getDigit());
        } while (Character.isDigit(reader.lookAheadChar()));
        return sb.toString();
    }

    /**
     * CHARS --> UNICODE*
     */
    public String getChars() throws Exception {
        StringBuilder sb = new StringBuilder();
        String follow3Payload = "}" + COM_BLOCK_IDEN_CLOSE;
        while (true) {
            String lookAhead3 = reader.lookAheadChar3();
            if (lookAhead3.equals(COM_LINE_IDEN) || lookAhead3.equals(COM_BLOCK_IDEN_OPEN)
                    || reader.lookAheadChar3().equals(follow3Payload))
                break;
            sb.append(getUnicode());
        }
        return sb.toString();
    }

    /**
     * CHARS_LIMITED --> (LETTER* DIGIT* SCHAR*)*
     */
    public String getCharsLimited() throws Exception {
        StringBuilder sb = new StringBuilder();
        char next = reader.lookAheadChar();
        while (Character.isLetter(next) || Character.isDigit(next) || isSChar(next)) {
            if (Character.isLetter(next)) {
                sb.append(getLetter());
            } else if (Character.isDigit(next)) {
                sb.append(getDigit());
            } else {
                sb.append(getSChar());
            }
            next = reader.lookAheadChar();
        }
        return sb.toString();
    }

    /**
     * LETTER --> a|b|...|y|z|A|B|...|Y|Z
     */
    public String getLetter() throws Exception {
        char next = reader.lookAheadChar();
        if (Character.isLetter(next)) {
            reader.advance();
            return String.valueOf(next);
        } else {
            throw new Exception("Expected letter");
        }
    }

    /**
     * DIGIT --> 0|1|2|3|4|5|6|7|8|9
     */
    public String getDigit() throws Exception {
        char next = reader.lookAheadChar();
        if (Character.isDigit(next)) {
            reader.advance();
            return String.valueOf(next);
        } else {
            throw new Exception("Expected digit");
        }
    }

    /**
     * SCHAR --> -| |.|_|[|]|{|}|/|\|'
     */
    public String getSChar() throws Exception {
        char next = reader.lookAheadChar();
        if (isSChar(next)) {
            reader.advance();
            return String.valueOf(next);
        } else {
            throw new Exception("Expected special character");
        }
    }

    /**
     * UNICODE --> Any unicode character
     *
     * @return
     * @throws Exception
     */
    public String getUnicode() throws Exception {
        char next = reader.lookAheadChar();
        if (isUnicode(next)) {
            reader.advance();
            return String.valueOf(next);
        } else {
            throw new Exception("Expected character");
        }
    }

    //////////////////////// UTILITIES /////////////////////////////////////////

    private boolean isUnicode(char next) {
        return Character.valueOf(next) instanceof Character;
    }

    private boolean isSChar(char next) {
        return next == '-' || next == '.' || next == '_' || next == '[' || next == ']' || next == '{' || next == '}'
                || next == '/' || next == '\\' || next == '\'';
    }

    private boolean isFollowIdentifier(char c) {
        return c == '=' || c == '!' || c == '|' || c == '{' || c == COM_LINE_IDEN.toCharArray()[0];
    }

    private boolean isFollowStatement(char c) {
        return c == '{' || c == COM_LINE_IDEN.toCharArray()[0];
    }

    private boolean isFollowChars(char c) {
        return c == '}' || c == COM_LINE_IDEN.toCharArray()[0];
    }

}
