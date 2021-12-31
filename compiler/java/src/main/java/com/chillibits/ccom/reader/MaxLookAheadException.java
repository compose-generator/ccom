/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.reader;

/**
 * Exception indicating that the max look ahead is less than 1.
 */
public class MaxLookAheadException extends Exception {

    public MaxLookAheadException(int maxLookAhead) {
        super("Max look ahead was " + maxLookAhead + " but must be at least 1");
    }

}
