//
// Created by Marc on 27.05.2021.
//

#include <reader/Reader.h>
#include "gtest/gtest.h"

// -------------------------------------------------------- EOF --------------------------------------------------------

TEST(ReaderTests, EoF) {
    Reader reader = Reader("Test", 2);
    EXPECT_EQ (reader.getLookahead(),  'T');
    reader.advance();
    reader.advance();
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  't');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
}

TEST(ReaderTests, EoFInEmptyFile) {
    Reader reader = Reader("", 5);
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
}

// ----------------------------------------------------- Lookahead -----------------------------------------------------

TEST(ReaderTests, Lookahead) {
    Reader reader = Reader("File input", 2);
    EXPECT_EQ (reader.getLookahead(),  'F');
}

TEST(ReaderTests, LookaheadAdvance) {
    Reader reader = Reader("File input", 2);
    EXPECT_EQ (reader.getLookahead(),  'F');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'i');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'l');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'e');
}

// --------------------------------------------------- Max lookahead ---------------------------------------------------

TEST(ReaderTests, MaxLookahead) {
    Reader reader = Reader("File input", 4);
    EXPECT_EQ (reader.getMaxLookahead(),  "File");
}

TEST(ReaderTests, MexLookaheadAdvance) {
    Reader reader = Reader("File input", 3);
    EXPECT_EQ (reader.getMaxLookahead(),  "Fil");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "ile");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "le ");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "e i");
}

TEST(ReaderTests, MaxLookaheadInvalid) {
    try {
        Reader reader = Reader("Test", 0);
        FAIL() << "Expected MaxLookaheadException";
    } catch(MaxLookaheadException const & err) {
        EXPECT_EQ(err.what(),std::string("Invalid maximum lookahead '0' specified. Must be >= 1"));
    }
}

TEST(ReaderTests, MaxLookaheadIsOne) {
    Reader reader = Reader("File input", 1);
    EXPECT_EQ (reader.getMaxLookahead(),  "F");
    EXPECT_EQ (reader.getMaxLookahead(),  std::string(1, reader.getLookahead()));
}

// ------------------------------------------------------- Expect ------------------------------------------------------

TEST(ReaderTests, ExpectSuccess) {
    Reader reader = Reader("File input", 4);
    try {
        reader.advance();
        reader.expect('i');
        reader.expect('l');
        reader.expect('e');
        reader.advance();
        reader.advance();
        reader.expect('n');
        reader.advance();
        reader.expect('u');
        SUCCEED();
    } catch (UnexpectedCharException const & err) {
        FAIL() << "Expected UnexpectedCharException got thrown";
    }
}

TEST(ReaderTests, ExpectFailure) {
    Reader reader = Reader("File input", 5);
    try {
        reader.advance();
        reader.expect('i');
        reader.expect('l');
        reader.expect('e');
        reader.advance();
        reader.expect('n');
        FAIL() << "Expected UnexpectedCharException";
    } catch (UnexpectedCharException const & err) {
        SUCCEED();
    }
}

// --------------------------------------------------- Expect Multiple -------------------------------------------------

TEST(ReaderTests, ExpectMultipleSuccess) {
    Reader reader = Reader("File input", 5);
    try {
        reader.advance();
        reader.expectMultiple("ile");
        reader.advance();
        reader.expectMultiple("in");
        reader.advance();
        reader.expectMultiple("ut");
        SUCCEED();
    } catch (UnexpectedCharException const & err) {
        FAIL() << "Expected UnexpectedCharException got thrown";
    }
}

TEST(ReaderTests, ExpectMultipleFailure) {
    Reader reader = Reader("File input", 5);
    try {
        reader.advance();
        reader.expectMultiple("ile");
        reader.advance();
        reader.expectMultiple("data");
        FAIL() << "Expected UnexpectedCharException";
    } catch (UnexpectedCharException const & err) {
        SUCCEED();
    }
}