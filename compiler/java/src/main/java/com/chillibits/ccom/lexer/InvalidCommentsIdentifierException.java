package com.chillibits.ccom.lexer;

/**
 * Exception indicating that the values for the comments identifier of the original language (e.g. Java) are invalid.
 */
public class InvalidCommentsIdentifierException extends Exception {

    public InvalidCommentsIdentifierException(String message) {
        super(message);
    }

}
