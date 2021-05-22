package com.c.filereader;

/**
 * Error indicating that the next char in the file did not equal the expected char.
 */
public class UnexpectedCharError extends Exception {

    public UnexpectedCharError(char expected, char got, String position) {
        super("Expected char '" + expected + "', but got '" + got + "' " + position);
    }

}
