package com.c.lexer;

import java.util.Objects;

public class Token {
    private final TokenType type;
    private String value;
    private final int posLine;
    private final int posCol;

    public Token(TokenType type, String value, int lineNum, int posCol) {
        this.type = type;
        this.value = value;
        this.posLine = lineNum;
        this.posCol = posCol;
    }

    public Token(TokenType type, int posLine, int posCol) {
        this.type = type;
        this.value = "";
        this.posLine = posLine;
        this.posCol = posCol;
    }

    public TokenType getType() {
        return type;
    }

    public String getValue() {
        return value;
    }

    /**
     * @return the code position where this Token occurs (line and column number)
     */
    public String getCodePos() {
        return "Line " + posLine + ",Col " + posCol;
    }

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
