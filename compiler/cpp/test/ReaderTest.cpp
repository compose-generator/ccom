//
// Created by Marc on 27.05.2021.
//

#include <reader/Reader.h>
#include "gtest/gtest.h"

TEST(ReaderTests, MaxLookahead) {
    Reader reader = Reader("File input", 4);
    EXPECT_EQ (reader.getMaxLookahead(),  "File");
}

TEST(ReaderTests, Lookahead) {
    Reader reader = Reader("File input", 2);
    EXPECT_EQ (reader.getLookahead(),  'F');
}

TEST(ReaderTests, AdvanceLookahead) {
    Reader reader = Reader("File input", 2);
    EXPECT_EQ (reader.getLookahead(),  'F');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'i');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'l');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'e');
}

TEST(ReaderTests, AdvanceMexLookahead) {
    Reader reader = Reader("File input", 3);
    EXPECT_EQ (reader.getMaxLookahead(),  "Fil");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "ile");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "le ");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "e i");
}