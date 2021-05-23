package com.c.lexer;

public enum TokenType {
    // End of file
    EOF,

    // Keywords
    IF,
    HAS,
    NOT,

    // Boolean values
    TRUE,
    FALSE,

    // Operators
    OR, // |
    EQUALS, // ==
    NOT_EQUALS, // !=
    GREATER, // >
    GREATER_EQUAL, // >=
    LESS_EQUAL, // <=

    // Data types
    IDENTIFIER, // e.g. myIdentifier
    NUMBER, // e.g. 3141592
    STRING, // e.g. "This is my string."

    // Misc
    DOT, // .
    BRACE_OPEN, // {
    BRACE_CLOSE, // }
    INDEX, // [42]
    // The following examples refer to Java
    COMMENT_PAYLOAD_IDENTIFIER, // //
    COMMENT_LINE_IDENTIFIER, // //?
    COMMENT_BLOCK_IDENTIFIER_OPEN, // /*?
    COMMENT_BLOCK_IDENTIFIER_CLOSE, // */

    // actual program code or data (in languages like JSON or yml)
    // // that is not relevant for us since we are enriching existent languages
    ARBITRARY
}
