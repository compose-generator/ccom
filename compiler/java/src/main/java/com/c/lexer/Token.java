package com.c.lexer;

public class Token {
    private TokenType tokenType;
    private String value;
    private int lineNum = 0;
    private int colNum = 0;

    public Token(TokenType tokenType, String value, int lineNum, int colNum) {
        this.tokenType = tokenType;
        this.value = value;
        this.lineNum = lineNum;
        this.colNum = colNum;
    }

    public TokenType getTokenType() {
        return tokenType;
    }

    public String getValue() {
        return value;
    }

    public String getCodePos() {
        return "Line " + lineNum + ",Col " + colNum;
    }

}
