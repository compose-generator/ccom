package com.c.lexer;

public class Lexer {

    private enum Context {
        PRORGRAM_CODE, // Strings before or after a conditional section
        SECTION, // Conditional section
        PAYLOAD // Payload content in a conditional section
    }

    public Token advance() {
        // TODO
        return null;
    }

}
