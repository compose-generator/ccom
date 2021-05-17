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

    // Data types
    IDENTIFIER, // e.g. myIdentifier
    NUMBER, // e.g. 3141592
    STRING, // e.g. "This is my string."

    // Misc
    DOT, // .
    BRACE_OPEN, // {
    BRACE_CLOSE, // }
    // The following examples refer to Java
    COMMENT_IDENTIFIER_PAYLOAD, // //
    COMMENT_LINE_IDENTIFIER, // //?
    COMMENT_BLOCK_IDENTIFIER_OPEN, // /*?
    COMMENT_BLOCK_IDENTIFIER_CLOSE, // */

    PROGRAM_CODE // actual program code that is not relevant for us since we are enriching the language
}
