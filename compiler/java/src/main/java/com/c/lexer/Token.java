package com.c.lexer;

public class Token {
    private TokenType type;
    private String value;
    private int lineNum;
    private int colNum;

    public Token(TokenType type, String value, int lineNum, int colNum) {
        this.type = type;
        this.value = value;
        this.lineNum = lineNum;
        this.colNum = colNum;
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
        return "Line " + lineNum + ",Col " + colNum;
    }

}
