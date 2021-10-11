/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.reader;

/**
 * Exception indicating that the next char in the file did not equal the expected char.
 */
public class UnexpectedCharException extends Exception {

    public UnexpectedCharException(char expected, char got, String position) {
        super("Expected '" + expected + "' but got '" + got + "' " + position);
    }

}
