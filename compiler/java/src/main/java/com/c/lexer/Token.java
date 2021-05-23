package com.c.lexer;

public class Token {
    private TokenType type;
    private String value;
    private int posLine;
    private int posCol;

    public Token(TokenType type, String value, int lineNum, int posCol) {
        this.type = type;
        this.value = value;
        this.posLine = lineNum;
        this.posCol = posCol;
    }

    public Token(TokenType type, int posLine, int posCol) {
        this.type = type;
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

}
