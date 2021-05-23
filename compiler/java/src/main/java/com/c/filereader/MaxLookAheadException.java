package com.c.filereader;

/**
 * Exception indicating that the max look ahead is less than 1.
 */
public class MaxLookAheadException extends Exception {

    public MaxLookAheadException(int maxLookAhead) {
        super("Max look ahead was " + maxLookAhead + " but must be at least 1");
    }

}
