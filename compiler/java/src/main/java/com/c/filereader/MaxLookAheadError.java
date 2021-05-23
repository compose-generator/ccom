package com.c.filereader;

public class MaxLookAheadError extends Exception {

    public MaxLookAheadError(int maxLookAhead) {
        super("Max look ahead was " + maxLookAhead + " but must be greater than 0");
    }

}
