package com.chillibits.ccom.lexer;

class AdvancedJavaTest {

    private void dummy() {
        int a = 5;
        int b = 6;
        int c = a + b;
        StringBuilder str = new StringBuilder("asdfjklö ASDFJKLÖ #+-'test' \"Test\" [2].3 4<5<6>=1>0 { test } // /* */ }*/");
        //?
        //
        //
        // if has not true false | == != <<= 36> >= testIdentifier 42 "TestString" 73. [124]{
        //String strValue = str.toString();
        // strValue += "//";
        //?}
        int arbitrary = 42;
        //? == teSt if {
        // int d = 5;
        //int e = 6;
        // int f = d + e;
        //?}
    }

    private void dummy2() {
        /*?
        "a" {
        StringBuilder str = new StringBuilder("/* { } //");
        }*/

        /*?"b" != < testIdentifier {
        StringBuilder str2 = new StringBuilder("/* { } //");
        int z = 42;
           }*/
    }

    private String rest() {
        return "This is the rest // /* */ // { }";
    }

}
