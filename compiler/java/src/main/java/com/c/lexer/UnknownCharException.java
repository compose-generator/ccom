package com.c.lexer;

public class UnknownCharException extends Exception {

    public UnknownCharException(char next) {
        super("Read an invalid char that cannot be processed to a valid Token: " + next);
    }

}
