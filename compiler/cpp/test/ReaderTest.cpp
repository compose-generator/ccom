//
// Created by Marc on 27.05.2021.
//

#include <reader/Reader.h>
#include <gtest/gtest.h>

// -------------------------------------------------------- EOF --------------------------------------------------------

TEST(ReaderTests, EoF) {
    // Initialize
    Reader reader = Reader("Test", 2);

    // Test
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
    // Initialize
    Reader reader = Reader("", 5);

    // Test
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  EOF);
}

// ----------------------------------------------------- Lookahead -----------------------------------------------------

TEST(ReaderTests, Lookahead) {
    // Initialize
    Reader reader = Reader("File input", 2);

    // Test
    EXPECT_EQ (reader.getLookahead(),  'F');
}

TEST(ReaderTests, LookaheadAdvance) {
    // Initialize
    Reader reader = Reader("File input", 2);

    // Test
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
    // Initialize
    Reader reader = Reader("File input", 4);

    // Test
    EXPECT_EQ (reader.getMaxLookahead(),  "File");
}

TEST(ReaderTests, MexLookaheadAdvance) {
    // Initialize
    Reader reader = Reader("File input", 3);

    // Test
    EXPECT_EQ (reader.getMaxLookahead(),  "Fil");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "ile");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "le ");
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "e i");
}

TEST(ReaderTests, MaxLookaheadInvalid) {
    // Initialize & test
    try {
        Reader reader = Reader("Test", 0);
        FAIL() << "Expected MaxLookaheadException";
    } catch(MaxLookaheadException const & err) {
        EXPECT_EQ(err.what(),std::string("Invalid maximum lookahead '0' specified. Must be >= 1"));
    }
}

TEST(ReaderTests, MaxLookaheadIsOne) {
    // Initialize
    Reader reader = Reader("File input", 1);

    // Test
    EXPECT_EQ (reader.getMaxLookahead(),  "F");
    EXPECT_EQ (reader.getMaxLookahead(),  std::string(1, reader.getLookahead()));
    reader.advance();
    reader.advance();
    reader.advance();
    EXPECT_EQ (reader.getMaxLookahead(),  "e");
    EXPECT_EQ (reader.getMaxLookahead(),  std::string(1, reader.getLookahead()));
}

// ------------------------------------------------------- Expect ------------------------------------------------------

TEST(ReaderTests, ExpectSuccess) {
    // Initialize
    Reader reader = Reader("File input", 4);

    // Test
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
    // Initialize
    Reader reader = Reader("File input", 5);

    // Test
    reader.advance();
    reader.expect('i');
    reader.expect('l');
    reader.expect('e');
    reader.advance();
    try {
        reader.expect('n');
        FAIL() << "Expected UnexpectedCharException";
    } catch (UnexpectedCharException const & err) {
        EXPECT_EQ(err.what(),std::string("Expected 'n', but got 'i' at L1 C6"));
    }
}

// --------------------------------------------------- Expect Multiple -------------------------------------------------

TEST(ReaderTests, ExpectMultipleSuccess) {
    // Initialize
    Reader reader = Reader("File input", 5);

    // Test
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
    // Initialize
    Reader reader = Reader("File input", 5);

    // Test
    reader.advance();
    reader.expectMultiple("ile");
    reader.advance();
    try {
        reader.expectMultiple("data");
        FAIL() << "Expected UnexpectedCharException";
    } catch (UnexpectedCharException const & err) {
        EXPECT_EQ(err.what(),std::string("Expected 'd', but got 'i' at L1 C6"));
    }
}

// ------------------------------------------------------- Code Pos ----------------------------------------------------

TEST(ReaderTests, CodePos) {
    // Initialize
    Reader reader = Reader("File input", 5);

    // Test
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 1);
    reader.advance();
    reader.advance();
    reader.advance();
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 4);
}

TEST(ReaderTests, CodePosLineBreaks) {
    // Initialize
    Reader reader = Reader("First\nSecond\n\nFourth", 5);

    // Test
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 1);
    reader.expectMultiple("First");
    reader.advance(); // Consume line break
    EXPECT_EQ(reader.getLineNum(), 2);
    EXPECT_EQ(reader.getColNum(), 1);
    reader.expectMultiple("Second");
    reader.advance(); // Consume line break
    reader.advance(); // Consume line break
    reader.expectMultiple("Fou");
    EXPECT_EQ(reader.getLineNum(), 4);
    EXPECT_EQ(reader.getColNum(), 4);
}

TEST(ReaderTests, CodePosEmptyFile) {
    // Initialize
    Reader reader = Reader("", 3);

    // Test
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 0);
    reader.advance();
    reader.advance();
    reader.advance();
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 0);
}