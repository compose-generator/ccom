package com.chillibits.ccom.lexer;

import java.util.Objects;

/**
 * A Token represents a building block of the CCom language, e.g. there is a token of type NUMBER to represent a number
 * like 42. Each Tokens stores a value with it although this value might be empty for fixed tokens like TRUE.
 * A Token also stores the position (line and column) where it occurs in the original file.
 * <p>
 * e.g. there is a Token IF, EQUAL for "==", NUMBER for a number like 42
 * * etc.
 */
public class Token {

    // ------------------------------------- Member variables ----------------------------------------------------------

    private final TokenType type;
    private final String value;

    // Position

    /**
     * The line in the original file where this Token occurs.
     */
    private final int posLine;

    /**
     * The column in the original file where this Token occurs.
     */
    private final int posCol;


    // ---------------------------------------- Constructor ------------------------------------------------------------

    /**
     * Constructs a new Token.
     *
     * @param type    type of the Token
     * @param value   value of the Token, e.g. a number as a string
     * @param posLine the line in the original file where this Token occurs
     * @param posCol  the column in the original file where this Token occurs
     */
    public Token(TokenType type, String value, int posLine, int posCol) {
        this.type = type;
        this.value = value;
        this.posLine = posLine;
        this.posCol = posCol;
    }


    /**
     * Constructs a new Token.
     *
     * @param type    type of the Token
     * @param posLine the line in the original file where this Token occurs
     * @param posCol  the column in the original file where this Token occurs
     */
    public Token(TokenType type, int posLine, int posCol) {
        this.type = type;
        this.value = "";
        this.posLine = posLine;
        this.posCol = posCol;
    }


    // ---------------------------------------- Type + Value -----------------------------------------------------------

    /**
     * @return type of this Token
     */
    public TokenType getType() {
        return type;
    }

    /**
     * @return value of this Token, e.g. a number as a string
     */
    public String getValue() {
        return value;
    }


    // ----------------------------------------- Position --------------------------------------------------------------

    /**
     * @return the code position where this Token occurs (line and column number)
     */
    public String getCodePos() {
        return "Line " + posLine + ",Col " + posCol;
    }

    /**
     * @return the line in the original file where this Token occurs.
     */
    public int getPosLine() {
        return posLine;
    }

    /**
     * @return the column in the original file where this Token occurs.
     */
    public int getPosCol() {
        return posCol;
    }


    // ------------------------------------ String comparison ----------------------------------------------------------

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Token token = (Token) o;
        return posLine == token.posLine && posCol == token.posCol && type == token.type && value.equals(token.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(type, value, posLine, posCol);
    }

    @Override
    public String toString() {
        return "Token{" +
                "type=" + type +
                ", value='" + value + '\'' +
                ", posLine=" + posLine +
                ", posCol=" + posCol +
                '}';
    }

}
