package com.c.filereader;

/**
 * Exception indicating that the max look ahead has an invalid value.
 */
public class MaxLookAheadException extends Exception {

    public MaxLookAheadException(int maxLookAhead) {
        super("Max look ahead was " + maxLookAhead + " but must be greater than 0");
    }

}
