package com.c.lexer;

import com.c.filereader.FileReader;
import com.c.filereader.MaxLookAheadException;
import com.c.filereader.UnexpectedCharException;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Lexer {

    // --------------------------------------- Member variables --------------------------------------------------------

    // File

    /**
     * A FileReader to read the file (given as String) char by char.
     */
    private final FileReader reader;


    // Buffer

    /**
     * Buffer for one Token.
     */
    private Token nextToken;

    // Context

    /**
     * Represents a state the lexer can be in. This is necessary since we only want to use the lexer for the conditional
     * sections and not for the arbitrary strings before or after a conditional section (Context.ARBITRARY) or for the
     * payload inside a conditional section (Context.PAYLOAD).
     */
    private enum Context {
        ARBITRARY, // Strings before or after a conditional section (program code or data)
        SECTION, // Conditional section
        PAYLOAD // Payload content in a conditional section
    }

    /**
     * The current context this Lexer is in.
     */
    private Context currentContext = Context.ARBITRARY;


    // Conditional comments chars

    /**
     * Identifier for line comments, e.g. in Java "//".
     */
    private final String commentLineIdentifier;

    /**
     * Identifier for opening block comments, e.g. in Java "/*".
     */
    private final String commentBlockOpenIdentifier;

    /**
     * Identifier for closing block comments, e.g. in Java "(star)/" where (star) is equal to *.
     */
    private final String commentBlockCloseIdentifier;

    /**
     * Identifier for the payload inside a conditional comments section.
     * Same as the line comment identifier. The payload comment identifier may also be an empty String.
     */
    private final String commentPayloadIdentifier;


    // --------------------------------------- Constructor -------------------------------------------------------------

    /**
     * Constructs a Lexer operating on a file given as String.
     * The Lexer is responsible to convert a stream of chars to a stream of Tokens.
     *
     * @param file                        the ipnut file as String
     * @param commentLineIdentifier       the identifier for line comments in the original language
     *                                    (e.g. // in Java)
     * @param commentBlockOpenIdentifier  the identifier for opening block comments in the original language
     *                                    (e.g. /* in Java)
     * @param commentBlockCloseIdentifier the identifier for closing block comments in the original language
     *                                    (e.g. (star)/ in Java where (star) is equal to *
     * @param isSingleStatement           whether the file consists only of a conditional comments section or a section embedded
     *                                    in a file that is accepted by another language (e.g. Java)
     * @throws InvalidCommentsIdentifierException if the comment identifiers are invalid
     *                                            (e.g. an identifier was provided for opening block comments but not for closing them)
     * @throws MaxLookAheadException              if the max look ahead is less than 1
     */
    public Lexer(String file,
                 String commentLineIdentifier,
                 String commentBlockOpenIdentifier,
                 String commentBlockCloseIdentifier,
                 boolean isSingleStatement) throws InvalidCommentsIdentifierException, MaxLookAheadException {

        // Check if combination of passed in comment identifiers is valid
        if (commentLineIdentifier.isEmpty() && commentBlockOpenIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You must provide at least line comments or block comments identifier");
        if (!commentBlockOpenIdentifier.isEmpty() && commentBlockCloseIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You provided an identifier to open block comments but not one to close them");
        if (!commentBlockCloseIdentifier.isEmpty() && commentBlockOpenIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You provided an identifier to close block comments but not one to open them");

        // Construct conditional comments identifiers based on comment identifiers of the underlying language (e.g. Java)
        this.commentLineIdentifier = commentLineIdentifier.isEmpty() ? "" : commentLineIdentifier + "?";
        this.commentBlockOpenIdentifier = commentBlockOpenIdentifier.isEmpty() ? "" : commentBlockCloseIdentifier + "?";
        this.commentBlockCloseIdentifier = commentBlockCloseIdentifier;
        this.commentPayloadIdentifier = commentLineIdentifier; // might be "", that's ok and considered in the grammar

        // Calculate max look ahead
        List<Integer> commentIdentifierLengths = new ArrayList<>();
        commentIdentifierLengths.add(commentLineIdentifier.length());
        commentIdentifierLengths.add(commentBlockOpenIdentifier.length());
        commentIdentifierLengths.add(commentBlockCloseIdentifier.length());
        commentIdentifierLengths.add(commentPayloadIdentifier.length());
        int maxLookAhead = Collections.max(commentIdentifierLengths);

        // Construct FileReader
        this.reader = new FileReader(file, maxLookAhead);
        currentContext = isSingleStatement ? Context.SECTION : Context.ARBITRARY;
    }


    // ----------------------------------------- Look ahead ------------------------------------------------------------

    /**
     * Looks ahead one token without consuming it.
     *
     * @return the next token
     */
    public Token lookAhead() {
        return nextToken;
    }


    // --------------------------------------- Expect ------------------------------------------------------------------

    /**
     * Checks if the next Token is the expected Token. Advances one token after a successful check.
     *
     * @param t expected Token (to compare the next Token against)
     * @throws UnexpectedTokenException if the next Token ís not equal to the expected Token
     */
    public void expect(Token t) throws UnexpectedTokenException, UnexpectedCharException, InvalidSectionException {
        if (!nextToken.equals(t))
            throw new UnexpectedTokenException(t, nextToken, reader.toPosString());
        advance();
    }


    // -------------------------------------- Advance ------------------------------------------------------------------

    /**
     * Consumes current Token and advances to next Token.
     *
     * @return the next token
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     * @throws InvalidSectionException if a char cannot be processed to a valid Token
     */
    public Token advance() throws InvalidSectionException, UnexpectedCharException {
        char nextChar = reader.lookAhead();

        // SKip whitespaces
        while (Character.isWhitespace(nextChar)) {
            reader.advance();
            nextChar = reader.lookAhead();
        }

        // Consume depending on context and get next Token
        switch (currentContext) {
            case ARBITRARY:
                nextToken = consumeArbitrary();
                break;
            case PAYLOAD:
                nextToken = consumePayload();
                break;
            case SECTION:
                nextToken = consumeSection();
        }

        advance();
        return nextToken;
    }

    /**
     * Consumes arbitrary chars, e.g. program code (e.g. Java) or data (e.g. JSON).
     *
     * @return a Token of type ARBITRARY and the char sequence as value of that Token
     */
    private Token consumeArbitrary() {
        StringBuilder value = new StringBuilder();
        while (!isLookAheadCommentLineIdentifier() && !isLookAheadCommentBlockOpenIdentifier() && !isEOF()) {
            value.append(reader.lookAhead());
            reader.advance();
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.ARBITRARY, value.toString());
    }

    /**
     * Consumes the payload that might get uncommented if the respective conditional comment evaluates to true.
     *
     * @return a Token of type ARBITRARY and the char sequence that might get uncommented as value of that Token
     */
    private Token consumePayload() {
        StringBuilder value = new StringBuilder();
        // TODO: do we allow the payload to include comments itself?
        while (!isLookAheadCommentLineIdentifier() && !isLookAheadCommentBlockCloseIdentifierWithBrace() && !isEOF()) {
            value.append(reader.lookAhead());
            reader.advance();
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.ARBITRARY, value.toString());
    }

    /**
     * Consumes part of a conditional comments section. One section can consist of multiple conditional comments,
     * e.g. multiple if statements. A conditional comments section is supposed to include a payload that might get
     * uncommented if the respective conditional comment evaluates to true.
     * <p>
     * Note that consumeSection() does NOT consume the whole section at once but step-by-step (with multiple calls to
     * this method). It constructs exactly one new token with every call to this method.
     *
     * @return a Token describing part of the conditional comments section, e.g. an "if" identifier
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     * @throws InvalidSectionException if a char cannot be processed to a valid Token
     */
    private Token consumeSection() throws UnexpectedCharException, InvalidSectionException {
        String nextChars = Arrays.toString(reader.lookAheads());

        if (isEOF()) return consumeEOF();

        switch (nextChars.charAt(0)) {
            case '|': // |
                return consumeOr();
            case '=': // ==
                return consumeEquals();
            case '!': // !=
                return consumeNotEquals();
            case '<': // < or <=
                return consumeLessThanOrLessEqualThan();
            case '>': // > or >=
                return consumeGreaterThanOrGreaterEqualThan();
            case '"': // string literal
                return consumeString();
            case '.': // .
                return consumeDot();
            case '{': // {
                return consumeBraceOpen();
            case '}': // }
                return consumeBraceClose();
            case '[': // [
                return consumeIndex();
        }

        if (Character.isAlphabetic(reader.lookAhead())) return consumeIdentifierOrKeyword();
        if (Character.isDigit(reader.lookAhead())) return consumeNumber();

        if (isLookAheadCommentLineIdentifier()) return consumeCommentLineIdentifier();
        if (isLookAheadCommentBlockOpenIdentifier()) return consumeCommentBlockOpenIdentifier();
        if (isLookAheadCommentBlockCloseIdentifier()) return consumeCommentBlockCloseIdentifier();
        if (isLookAheadCommentPayloadIdentifier()) return consumeCommentPayloadIdentifier();

        throw new InvalidSectionException(reader.lookAhead());
    }


    // ---------------------------------------- Check Token Type -------------------------------------------------------

    /**
     * @return ture iff the next char is EOF (end of file)
     */
    private boolean isEOF() {
        return reader.lookAhead() == (char) -1;
    }

    /**
     * @return true iff there exists a line comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentLineIdentifier() {
        return !commentLineIdentifier.isEmpty() && Arrays.toString(reader.lookAheads()).startsWith(commentLineIdentifier);
    }

    /**
     * @return true iff there exists an open block comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentBlockOpenIdentifier() {
        return !commentBlockOpenIdentifier.isEmpty() && Arrays.toString(reader.lookAheads()).startsWith(commentBlockOpenIdentifier);
    }

    /**
     * @return true iff there exists an close block comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentBlockCloseIdentifier() {
        return !commentBlockCloseIdentifier.isEmpty() && Arrays.toString(reader.lookAheads()).startsWith(commentBlockCloseIdentifier);
    }

    /**
     * @return true iff there exists an payload comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentPayloadIdentifier() {
        return !commentPayloadIdentifier.isEmpty() && Arrays.toString(reader.lookAheads()).startsWith(commentPayloadIdentifier);
    }

    /**
     * @return true iff there exists an close block comment identifier and the next chars are equal to:
     * "}" + the close block comment identifier
     */
    private boolean isLookAheadCommentBlockCloseIdentifierWithBrace() {
        return !commentBlockCloseIdentifier.isEmpty() && Arrays.toString(reader.lookAheads()).startsWith("}" + commentBlockCloseIdentifier);
    }


    // ------------------------------------------ Consume --------------------------------------------------------------

    /**
     * Consumes EOF (end of file).
     *
     * @return a Token representing EOF
     */
    private Token consumeEOF() {
        return constructTokenAtCurrentPosition(TokenType.EOF);
    }

    /**
     * Consumes a line comment identifier (e.g. in Java "//").
     * This will switch to the SECTION context.
     *
     * @return a Token representing that line comment identifier
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeCommentLineIdentifier() throws UnexpectedCharException {
        for (char c : commentLineIdentifier.toCharArray()) {
            reader.expect(c);
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.COMMENT_LINE_IDENTIFIER);
    }

    /**
     * Consumes an open block comment identifier (e.g. in Java "/*").
     * This will switch to the SECTION context.
     *
     * @return a Token representing that open block comment identifier
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeCommentBlockOpenIdentifier() throws UnexpectedCharException {
        for (char c : commentBlockOpenIdentifier.toCharArray()) {
            reader.expect(c);
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER);
    }

    /**
     * Consumes a close block comment identifier (e.g. in Java "(star)/" where (star) is equal to *).
     * This will switch to the SECTION context.
     *
     * @return a Token representing that close block comment identifier
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeCommentBlockCloseIdentifier() throws UnexpectedCharException {
        for (char c : commentBlockCloseIdentifier.toCharArray()) {
            reader.expect(c);
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER);
    }

    /**
     * Consumes a payload comment identifier (same identifier as line comments).
     * This will switch to the SECTION context.
     *
     * @return a Token representing that payload comment identifier
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeCommentPayloadIdentifier() throws UnexpectedCharException {
        for (char c : commentPayloadIdentifier.toCharArray()) {
            reader.expect(c);
        }
        currentContext = Context.SECTION;
        return constructTokenAtCurrentPosition(TokenType.COMMENT_PAYLOAD_IDENTIFIER);
    }

    /**
     * Consumes a "|" operator in a condition, e.g. "if a | b" (spoken: "if a or b").
     *
     * @return a Token representing that "|" operator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeOr() throws UnexpectedCharException {
        reader.expect('|');
        return constructTokenAtCurrentPosition(TokenType.OR);
    }

    /**
     * Consumes a "=" operator in a condition, e.g. "if a == b" (spoken: "if a equals b").
     *
     * @return a Token representing that "=" operator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeEquals() throws UnexpectedCharException {
        reader.expect('=');
        reader.expect('=');
        return constructTokenAtCurrentPosition(TokenType.EQUALS);
    }

    /**
     * Consumes a "!=" operator in a condition, e.g. "if a != b" (spoken: "if a does not equal b").
     *
     * @return a Token representing that "!=" operator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeNotEquals() throws UnexpectedCharException {
        reader.expect('!');
        reader.expect('=');
        return constructTokenAtCurrentPosition(TokenType.NOT_EQUALS);
    }

    /**
     * Consumes a "<" or a "<=" operator in a condition,
     * e.g. "if a < b" (spoken: "if a is less than b")
     * or "if a <= b" (spoken: "if a is less or equal b").
     *
     * @return a Token representing that "<" or "<=" operator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeLessThanOrLessEqualThan() throws UnexpectedCharException {
        reader.expect('<');
        if (reader.lookAhead() == '=') { // <=
            reader.expect('=');
            return constructTokenAtCurrentPosition(TokenType.LESS_EQUAL);
        }
        return constructTokenAtCurrentPosition(TokenType.LESS);
    }

    /**
     * Consumes a ">" or a ">=" operator in a condition,
     * e.g. "if a > b" (spoken: "if a is greater than b")
     * or "if a >= b" (spoken: "if a is greater or equal b").
     *
     * @return a Token representing that ">" or ">=" operator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeGreaterThanOrGreaterEqualThan() throws UnexpectedCharException {
        reader.expect('>');
        if (reader.lookAhead() == '=') { // >=
            reader.expect('=');
            return constructTokenAtCurrentPosition(TokenType.GREATER_EQUAL);
        }
        return constructTokenAtCurrentPosition(TokenType.GREATER);
    }

    /**
     * Consumes a "." separator in an identifier, e.g. "a.b[1].c".
     *
     * @return a Token representing that "." separator
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeDot() throws UnexpectedCharException {
        reader.expect('.');
        return constructTokenAtCurrentPosition(TokenType.DOT);
    }

    /**
     * Consumes a "{" char, e.g. "abc { def".
     * This will switch to the PAYLOAD context.
     *
     * @return a Token representing that "{" char
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeBraceOpen() throws UnexpectedCharException {
        reader.expect('{');
        currentContext = Context.PAYLOAD;
        return constructTokenAtCurrentPosition(TokenType.BRACE_OPEN);
    }

    /**
     * Consumes a "}" char, e.g. "def } ghi".
     * This will switch to the ARBITRARY context.
     *
     * @return a Token representing that "}" char
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeBraceClose() throws UnexpectedCharException {
        reader.expect('{');
        currentContext = Context.ARBITRARY;
        return constructTokenAtCurrentPosition(TokenType.BRACE_CLOSE);
    }

    /**
     * Consumes an index, e.g. in "a.b[42].c" ("[42]" is the index)
     *
     * @return a Token representing the index (including the value of the index as String)
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeIndex() throws UnexpectedCharException {
        reader.expect('[');
        Token numberToken = consumeNumber();
        reader.expect(']');
        return constructTokenAtCurrentPosition(TokenType.INDEX, numberToken.getValue());
    }

    /**
     * Consumes a number, e.g. "3141592".
     *
     * @return a Token representing that number (including its value as String)
     */
    private Token consumeNumber() {
        StringBuilder value = new StringBuilder();
        do {
            value.append(reader.lookAhead());
            reader.advance();
        } while (Character.isDigit(reader.lookAhead()));

        return constructTokenAtCurrentPosition(TokenType.NUMBER, value.toString());
    }

    /**
     * Consumes a String literal, e.g. "My String"
     * (note that these are "real" double quotes now that appear as such in the data).
     *
     * @return a Token representing that String literal (including its value)
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeString() throws UnexpectedCharException {
        reader.expect('"');
        StringBuilder value = new StringBuilder();
        while (reader.lookAhead() != '"' && reader.lookAhead() != (char) -1) {
            // Consider escaped characters
            // e.g "This is \"quote\"" or "This is a \\n line break in a string literal"
            if (reader.lookAhead() == '\\') {
                reader.advance(); // one extra advance
            }
            value.append(reader.lookAhead());
            reader.advance();
        }
        reader.expect('"');
        return constructTokenAtCurrentPosition(TokenType.STRING, value.toString());
    }

    /**
     * Consumes an identifier (e.g. "myIdentifier") or a keyword (e.g. "if", "has", "true" etc.).
     *
     * @return a Token representing the identifier (and its name stored as value in the Token) or a keyword.
     * @throws UnexpectedCharException if a char - read by the FileReader - was not the expected one
     */
    private Token consumeIdentifierOrKeyword() throws UnexpectedCharException {
        StringBuilder value = new StringBuilder(reader.lookAhead());
        reader.advance();
        while (Character.isLetterOrDigit(reader.lookAhead())) { // [a-zA-Z0-9]*
            value.append(reader.lookAhead());
        }

        // Is keyword?
        String identifier = value.toString();
        if (identifier.equals("if")) return constructTokenAtCurrentPosition(TokenType.IF);
        if (identifier.equals("has")) return constructTokenAtCurrentPosition(TokenType.HAS);
        if (identifier.equals("not")) return constructTokenAtCurrentPosition(TokenType.NOT);
        if (identifier.equals("true")) return constructTokenAtCurrentPosition(TokenType.TRUE);
        if (identifier.equals("false")) return constructTokenAtCurrentPosition(TokenType.FALSE);

        return constructTokenAtCurrentPosition(TokenType.IDENTIFIER, identifier);
    }


    // ------------------------------------------ Util -----------------------------------------------------------------

    /**
     * Constructs a Token based on the current position.
     *
     * @param type type of the token
     * @return a new Token
     */
    private Token constructTokenAtCurrentPosition(TokenType type) {
        return new Token(type, reader.getPosLine(), reader.getPosCol());
    }

    /**
     * Constructs a Token based on the current position.
     *
     * @param type  type of the token
     * @param value the String that the Token is supposed to store
     * @return a new Token
     */
    private Token constructTokenAtCurrentPosition(TokenType type, String value) {
        return new Token(type, value, reader.getPosLine(), reader.getPosCol());
    }

}
