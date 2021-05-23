package com.c.lexer;

public class InvalidSectionException extends Exception {

    public InvalidSectionException(char next) {
        super("Read an invalid char that cannot be processed to a valid Token: " + next);
    }

}
