/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

package com.chillibits.ccom.lexer;

/**
 * Exception indicating that the next Token did not equal the expected Token.
 */
public class UnexpectedTokenException extends Exception {

    public UnexpectedTokenException(Token expected, Token got, String position) {
        super("Expected '" + expected.getType() + "' but got '" + got.getType() + "' " + position);
    }

}
