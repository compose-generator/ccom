package com.c.lexer;

public class Lexer {

    // --- Comment chars
    private final String commentLineIdentifier;
    private final String commentBlockIdentifierOpen;
    private final String commentBlockIdentifierClose;

    private enum Context {
        ARBITRARY, // Strings before or after a conditional section
        SECTION, // Conditional section
        PAYLOAD // Payload content in a conditional section
    }

    public Lexer(String commentLineIdentifier, String commentBlockIdentifierOpen, String commentBlockIdentifierClose) {
        // Build conditional comments, based on comment chars input
        this.commentLineIdentifier = commentLineIdentifier;
        this.commentBlockIdentifierOpen = commentBlockIdentifierOpen;
        this.commentBlockIdentifierClose = commentBlockIdentifierClose;
    }

    public void advance() {
        // TODO
    }

    // TODO

}
