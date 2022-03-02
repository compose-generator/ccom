/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.lexer;

import com.chillibits.ccom.Constants;
import com.chillibits.ccom.reader.MaxLookAheadException;
import com.chillibits.ccom.reader.Reader;
import com.chillibits.ccom.reader.UnexpectedCharException;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Uses a Reader to operate on a char sequence and sequentially transforms this sequence into {@link Token}s.
 * This Lexer provides an API to advance() with the current position and to expect() a specific Token or TokenType.
 */
public class Lexer {

    // --------------------------------------- Member variables --------------------------------------------------------

    // File

    /**
     * A Reader to read the file (given as String) char by char.
     */
    private final Reader reader;


    // Buffer

    /**
     * Buffer for one Token.
     */
    private Token nextToken;

    /**
     * Buffer for the start position (line number) of the next Token.
     */
    private int tokenStartPosLine;

    /**
     * Buffer for the start position (col number) of the next Token.
     */
    private int tokenStartPosCol;


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
    private Context currentContext;


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
     * @param file                        the input file as String
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
     * @throws UnexpectedCharException            if a char - read by the Reader - was not the expected one
     * @throws UnknownCharException               if a char cannot be processed to a valid Token
     */
    public Lexer(String file,
                 String commentLineIdentifier,
                 String commentBlockOpenIdentifier,
                 String commentBlockCloseIdentifier,
                 boolean isSingleStatement) throws InvalidCommentsIdentifierException, MaxLookAheadException, UnexpectedCharException, UnknownCharException {

        // Check if combination of passed in comment identifiers is valid
        if (!commentBlockOpenIdentifier.isEmpty() && commentBlockCloseIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You provided an identifier to open block comments but not one to close them");
        if (!commentBlockCloseIdentifier.isEmpty() && commentBlockOpenIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You provided an identifier to close block comments but not one to open them");
        if (commentLineIdentifier.isEmpty() && commentBlockOpenIdentifier.isEmpty())
            throw new InvalidCommentsIdentifierException("You must provide at least line comments or block comments identifier");

        // Construct conditional comments identifiers based on comment identifiers of the underlying language (e.g. Java)
        this.commentLineIdentifier = commentLineIdentifier.isEmpty() ? "" : commentLineIdentifier + "?";
        this.commentBlockOpenIdentifier = commentBlockOpenIdentifier.isEmpty() ? "" : commentBlockOpenIdentifier + "?";
        this.commentBlockCloseIdentifier = commentBlockCloseIdentifier;
        this.commentPayloadIdentifier = commentLineIdentifier; // might be "", that's ok and considered in the grammar

        // Calculate max look ahead
        List<Integer> commentIdentifierLengths = new ArrayList<>();
        commentIdentifierLengths.add(commentLineIdentifier.length());
        commentIdentifierLengths.add(commentBlockOpenIdentifier.length());
        commentIdentifierLengths.add(commentBlockCloseIdentifier.length());
        commentIdentifierLengths.add(commentPayloadIdentifier.length());
        // We need one character more than the max for the method isLookAheadCommentBlockCloseIdentifierWithBrace()
        int maxLookAhead = Collections.max(commentIdentifierLengths) + 1;

        // Construct Reader
        this.reader = new Reader(file, maxLookAhead);

        // Set context
        currentContext = isSingleStatement || isLookAheadCommentLineIdentifier() || isLookAheadCommentBlockOpenIdentifier()
                ? Context.SECTION : Context.ARBITRARY;

        advance();
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
     * @throws UnexpectedCharException  if a char - read by the Reader - was not the expected one
     * @throws UnknownCharException     if a char cannot be processed to a valid Token
     */
    public void expect(Token t) throws UnexpectedTokenException, UnexpectedCharException, UnknownCharException {
        if (!nextToken.equals(t))
            throw new UnexpectedTokenException(t, nextToken, reader.toPosString());
        advance();
    }


    // -------------------------------------- Advance ------------------------------------------------------------------

    /**
     * Consumes current Token and advances to next Token.
     *
     * @return the next token
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     * @throws UnknownCharException    if a char cannot be processed to a valid Token
     */
    public Token advance() throws UnknownCharException, UnexpectedCharException {
        // Skip whitespaces
        while (currentContext != Context.ARBITRARY && Character.isWhitespace(reader.lookAhead())) {
            reader.advance();
        }
        updateTokenStartPosition();

        // EOF?
        if (isEOF()) {
            nextToken = consumeEOF();
            return nextToken;
        }

        // Consume depending on context and get next Token
        switch (currentContext) {
            case ARBITRARY -> nextToken = consumeArbitrary();
            case PAYLOAD -> nextToken = consumePayload();
            case SECTION -> nextToken = consumeSection();
        }

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
        return constructToken(TokenType.ARBITRARY, value.toString());
    }

    /**
     * Consumes the payload that might get uncommented if the respective conditional comment evaluates to true.
     *
     * @return a Token of type ARBITRARY and the char sequence that might get uncommented as value of that Token
     */
    private Token consumePayload() throws UnexpectedCharException {
        StringBuilder value = new StringBuilder();
        while (!isLookAheadCommentLineIdentifier()
                && !isLookAheadCommentBlockCloseIdentifierWithBrace()
                && !isEOF()) {

            // Ignore payload comment identifiers
            if (isLookAheadCommentPayloadIdentifier()) {
                consumeCommentPayloadIdentifier();
            }

            // TODO: also allow for any chars after '}' and before '-->' (HTML)
            // However, upon first evaluation, this would long for an infinite look ahead

            value.append(reader.lookAhead());
            reader.advance();
        }

        currentContext = Context.SECTION;
        return constructToken(TokenType.ARBITRARY, value.toString());
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
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     * @throws UnknownCharException    if a char cannot be processed to a valid Token
     */
    private Token consumeSection() throws UnexpectedCharException, UnknownCharException {
        // Check for Tokens first that require more than one character look ahead (!)
        if (isLookAheadCommentLineIdentifier()) return consumeCommentLineIdentifier();
        if (isLookAheadCommentBlockOpenIdentifier()) return consumeCommentBlockOpenIdentifier();
        if (isLookAheadCommentBlockCloseIdentifier()) return consumeCommentBlockCloseIdentifier();
        if (isLookAheadCommentPayloadIdentifier()) return consumeCommentPayloadIdentifier();

        char nextChar = reader.lookAhead();

        switch (nextChar) {
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

        if (Character.isAlphabetic(nextChar)) return consumeIdentifierOrKeyword();
        if (Character.isDigit(nextChar)) return consumeNumber();

        throw new UnknownCharException(nextChar, reader.toPosString());
    }


    // ---------------------------------------- Check Token Type -------------------------------------------------------

    /**
     * @return ture iff the next char is EOF (end of file)
     */
    private boolean isEOF() {
        return reader.lookAhead() == Constants.EOF;
    }

    /**
     * @return true iff there exists a line comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentLineIdentifier() {
        return !commentLineIdentifier.isEmpty() && reader.lookAheads().startsWith(commentLineIdentifier);
    }

    /**
     * @return true iff there exists an open block comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentBlockOpenIdentifier() {
        return !commentBlockOpenIdentifier.isEmpty() && reader.lookAheads().startsWith(commentBlockOpenIdentifier);
    }

    /**
     * @return true iff there exists an close block comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentBlockCloseIdentifier() {
        return !commentBlockCloseIdentifier.isEmpty() && reader.lookAheads().startsWith(commentBlockCloseIdentifier);
    }

    /**
     * @return true iff there exists an payload comment identifier and the next chars are equal to it
     */
    private boolean isLookAheadCommentPayloadIdentifier() {
        return !commentPayloadIdentifier.isEmpty() && reader.lookAheads().startsWith(commentPayloadIdentifier);
    }

    /**
     * @return true iff there exists an close block comment identifier and the next chars are equal to:
     * "}" + the close block comment identifier
     */
    private boolean isLookAheadCommentBlockCloseIdentifierWithBrace() {
        return !commentBlockCloseIdentifier.isEmpty() && reader.lookAheads().startsWith("}" + commentBlockCloseIdentifier);
    }


    // ------------------------------------------ Consume --------------------------------------------------------------

    /**
     * Consumes EOF (end of file).
     *
     * @return a Token representing EOF
     */
    private Token consumeEOF() {
        return constructToken(TokenType.EOF);
    }

    /**
     * Consumes a line comment identifier (e.g. in Java "//").
     * This will switch to the SECTION context.
     *
     * @return a Token representing that line comment identifier
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeCommentLineIdentifier() throws UnexpectedCharException {
        reader.expectMultiple(commentLineIdentifier);
        currentContext = Context.SECTION;
        return constructToken(TokenType.COMMENT_LINE_IDENTIFIER);
    }

    /**
     * Consumes an open block comment identifier (e.g. in Java "/*").
     * This will switch to the SECTION context.
     *
     * @return a Token representing that open block comment identifier
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeCommentBlockOpenIdentifier() throws UnexpectedCharException {
        reader.expectMultiple(commentBlockOpenIdentifier);
        currentContext = Context.SECTION;
        return constructToken(TokenType.COMMENT_BLOCK_OPEN_IDENTIFIER);
    }

    /**
     * Consumes a close block comment identifier (e.g. in Java "(star)/" where (star) is equal to *).
     * This will switch to the SECTION context.
     *
     * @return a Token representing that close block comment identifier
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeCommentBlockCloseIdentifier() throws UnexpectedCharException {
        reader.expectMultiple(commentBlockCloseIdentifier);
        currentContext = Context.ARBITRARY;
        return constructToken(TokenType.COMMENT_BLOCK_CLOSE_IDENTIFIER);
    }

    /**
     * Consumes a payload comment identifier (same identifier as line comments).
     * This will switch to the SECTION context.
     *
     * @return a Token representing that payload comment identifier
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeCommentPayloadIdentifier() throws UnexpectedCharException {
        reader.expectMultiple(commentPayloadIdentifier);
        return constructToken(TokenType.COMMENT_PAYLOAD_IDENTIFIER);
    }

    /**
     * Consumes a "|" operator in a condition, e.g. "if a | b" (spoken: "if a or b").
     *
     * @return a Token representing that "|" operator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeOr() throws UnexpectedCharException {
        reader.expect('|');
        return constructToken(TokenType.OR);
    }

    /**
     * Consumes a "=" operator in a condition, e.g. "if a == b" (spoken: "if a equals b").
     *
     * @return a Token representing that "=" operator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeEquals() throws UnexpectedCharException {
        reader.expect('=');
        reader.expect('=');
        return constructToken(TokenType.EQUALS);
    }

    /**
     * Consumes a "!=" operator in a condition, e.g. "if a != b" (spoken: "if a does not equal b").
     *
     * @return a Token representing that "!=" operator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeNotEquals() throws UnexpectedCharException {
        reader.expect('!');
        reader.expect('=');
        return constructToken(TokenType.NOT_EQUALS);
    }

    /**
     * Consumes a "<" or a "<=" operator in a condition,
     * e.g. "if a < b" (spoken: "if a is less than b")
     * or "if a <= b" (spoken: "if a is less or equal b").
     *
     * @return a Token representing that "<" or "<=" operator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeLessThanOrLessEqualThan() throws UnexpectedCharException {
        reader.expect('<');
        if (reader.lookAhead() == '=') { // <=
            reader.expect('=');
            return constructToken(TokenType.LESS_EQUAL);
        }
        return constructToken(TokenType.LESS);
    }

    /**
     * Consumes a ">" or a ">=" operator in a condition,
     * e.g. "if a > b" (spoken: "if a is greater than b")
     * or "if a >= b" (spoken: "if a is greater or equal b").
     *
     * @return a Token representing that ">" or ">=" operator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeGreaterThanOrGreaterEqualThan() throws UnexpectedCharException {
        reader.expect('>');
        if (reader.lookAhead() == '=') { // >=
            reader.expect('=');
            return constructToken(TokenType.GREATER_EQUAL);
        }
        return constructToken(TokenType.GREATER);
    }

    /**
     * Consumes a "." separator in an identifier, e.g. "a.b[1].c".
     *
     * @return a Token representing that "." separator
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeDot() throws UnexpectedCharException {
        reader.expect('.');
        return constructToken(TokenType.DOT);
    }

    /**
     * Consumes a "{" char, e.g. "abc { def".
     * This will switch to the PAYLOAD context.
     *
     * @return a Token representing that "{" char
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeBraceOpen() throws UnexpectedCharException {
        reader.expect('{');
        currentContext = Context.PAYLOAD;
        return constructToken(TokenType.BRACE_OPEN);
    }

    /**
     * Consumes a "}" char, e.g. "def } ghi".
     * This will switch to the ARBITRARY context.
     *
     * @return a Token representing that "}" char
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeBraceClose() throws UnexpectedCharException {
        reader.expect('}');
        if (!isLookAheadCommentBlockCloseIdentifier()) currentContext = Context.ARBITRARY;
        return constructToken(TokenType.BRACE_CLOSE);
    }

    /**
     * Consumes an index, e.g. in "a.b[42].c" ("[42]" is the index)
     *
     * @return a Token representing the index (including the value of the index as String)
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeIndex() throws UnexpectedCharException {
        reader.expect('[');
        Token numberToken = consumeNumber();
        reader.expect(']');
        return constructToken(TokenType.INDEX, numberToken.getValue());
    }

    /**
     * Consumes a number, e.g. "3141592".
     *
     * @return a Token representing that number (including its value as String)
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeNumber() throws UnexpectedCharException {
        StringBuilder value = new StringBuilder();
        do {
            value.append(reader.lookAhead());
            reader.advance();
        } while (Character.isDigit(reader.lookAhead()));
        return constructToken(TokenType.NUMBER, value.toString());
    }

    /**
     * Consumes a String literal, e.g. "My String"
     * (note that these are "real" double quotes now that appear as such in the data).
     *
     * @return a Token representing that String literal (including its value)
     * @throws UnexpectedCharException if a char - read by the Reader - was not the expected one
     */
    private Token consumeString() throws UnexpectedCharException {
        reader.expect('"');

        StringBuilder value = new StringBuilder();
        while (reader.lookAhead() != '"' && reader.lookAhead() != Constants.EOF) {

            // Consider escaped characters
            // e.g "This is \"quote\"" or "This is a \\n line break in a string literal"
            if (reader.lookAhead() == '\\') {
                reader.advance(); // one extra advance
            }

            value.append(reader.lookAhead());
            reader.advance();
        }

        reader.expect('"');

        return constructToken(TokenType.STRING, value.toString());
    }

    /**
     * Consumes an identifier (e.g. "myIdentifier") or a keyword (e.g. "if", "has", "true" etc.).
     *
     * @return a Token representing the identifier (and its name stored as value in the Token) or a keyword.
     */
    private Token consumeIdentifierOrKeyword() {
        StringBuilder value = new StringBuilder(String.valueOf(reader.lookAhead()));
        reader.advance();
        while (Character.isLetterOrDigit(reader.lookAhead())) { // [a-zA-Z0-9]*
            value.append(reader.lookAhead());
            reader.advance();
        }

        // Is keyword?
        String identifier = value.toString();
        if (identifier.equals("if")) return constructToken(TokenType.IF);
        if (identifier.equals("has")) return constructToken(TokenType.HAS);
        if (identifier.equals("not")) return constructToken(TokenType.NOT);
        if (identifier.equals("true")) return constructToken(TokenType.TRUE);
        if (identifier.equals("false")) return constructToken(TokenType.FALSE);

        return constructToken(TokenType.IDENTIFIER, identifier);
    }


    // ------------------------------------------- Util ----------------------------------------------------------------

    /**
     * Updates the buffered Token start position.
     */
    private void updateTokenStartPosition() {
        this.tokenStartPosLine = reader.getPosLine();
        this.tokenStartPosCol = reader.getPosCol();
    }

    /**
     * Constructs a Token on the current Token start position.
     *
     * @param type type of the Token
     * @return a new Token
     */
    private Token constructToken(TokenType type) {
        return new Token(type, tokenStartPosLine, tokenStartPosCol);
    }

    /**
     * Constructs a Token on the current Token start position.
     *
     * @param type  type of the Token
     * @param value value to be stored in the Token
     * @return a new Token
     */
    private Token constructToken(TokenType type, String value) {
        return new Token(type, value, tokenStartPosLine, tokenStartPosCol);
    }

}
