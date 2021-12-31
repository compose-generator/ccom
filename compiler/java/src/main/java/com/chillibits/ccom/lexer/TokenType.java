/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.lexer;

/**
 * Describes the possible types of a Token, e.g. EOF, IF, EQUALS, NUMBER etc.
 */
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
    LESS, // <
    LESS_EQUAL, // <=
    GREATER, // >
    GREATER_EQUAL, // >=

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
    COMMENT_LINE_IDENTIFIER, // //?
    COMMENT_BLOCK_OPEN_IDENTIFIER, // /*?
    COMMENT_BLOCK_CLOSE_IDENTIFIER, // */
    COMMENT_PAYLOAD_IDENTIFIER, // //

    // actual program code or data (in languages like JSON or yml)
    // that is not relevant for us since we are enriching existent languages
    ARBITRARY // e.g. asd'!?fowen7a_=sdfkh%package class { public int main() {} }"
}
