/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.lexer;

public class UnknownCharException extends Exception {

    public UnknownCharException(char next, String position) {
        super("Read an invalid char that cannot be processed to a valid Token: " + next + " " + position);
    }

}
