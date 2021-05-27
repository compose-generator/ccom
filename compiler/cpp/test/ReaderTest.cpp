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
    reader.expectMultiple("Tes");
    EXPECT_EQ (reader.getLookahead(),  't');
    reader.expect('t');
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.expect(EOF);
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.expect(EOF);
}

TEST(ReaderTests, EoFInEmptyFile) {
    // Initialize
    Reader reader = Reader("", 2);

    // Test
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.expect(EOF);
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.expect(EOF);
    EXPECT_EQ (reader.getLookahead(),  EOF);
    reader.expect(EOF);
}

// ----------------------------------------------------- Lookahead -----------------------------------------------------

TEST(ReaderTests, Lookahead) {
    // Initialize
    Reader reader = Reader("File input", 2);

    // Test
    EXPECT_EQ (reader.getLookahead(),  'F');
    reader.expect('F');
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
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  ' ');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'i');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'n');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'p');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  'u');
    reader.advance();
    EXPECT_EQ (reader.getLookahead(),  't');
}

TEST(ReaderTests, LookaheadWithLinebreaks) {
    // Initialize
    Reader reader = Reader("\n\nFile\n\n\n\ninput\n", 2);

    // Test#
    reader.expect('\n');
    reader.expect('\n');
    EXPECT_EQ (reader.getLookahead(),  'F');
    reader.expectMultiple("File");
    reader.expectMultiple("\n\n\n\n");
    EXPECT_EQ (reader.getLookahead(),  'i');
    reader.expectMultiple("input");
    EXPECT_EQ (reader.getLookahead(),  '\n');
    reader.expect('\n');
}

// --------------------------------------------------- Max lookahead ---------------------------------------------------

TEST(ReaderTests, LookaheadMultiple) {
    // Initialize
    Reader reader = Reader("File input", 4);

    // Test
    EXPECT_EQ (reader.getLookaheadMultiple(),  "File");
    reader.expectMultiple("File");
}

TEST(ReaderTests, LookaheadMultipleAdvance) {
    // Initialize
    Reader reader = Reader("File input", 3);

    // Test
    EXPECT_EQ (reader.getLookaheadMultiple(),  "Fil");
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "ile");
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "le ");
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "e i");
    reader.advance();
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "inp");
    reader.advance();
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "put");
}

TEST(ReaderTests, LookaheadMultipleInvalid) {
    // Initialize & test
    try {
        Reader reader = Reader("Test", 0);
        FAIL() << "Expected MaxLookaheadException";
    } catch(MaxLookaheadException const & err) {
        EXPECT_EQ(err.what(),std::string("Invalid maximum lookahead '0' specified. Must be >= 1"));
    }
}

TEST(ReaderTests, LookaheadMultipleIsOne) {
    // Initialize
    Reader reader = Reader("File input", 1);

    // Test
    EXPECT_EQ (reader.getLookaheadMultiple(),  "F");
    EXPECT_EQ (reader.getLookaheadMultiple(),  std::string(1, reader.getLookahead()));
    reader.expect('F');
    reader.advance();
    reader.advance();
    EXPECT_EQ (reader.getLookaheadMultiple(),  "e");
    EXPECT_EQ (reader.getLookaheadMultiple(),  std::string(1, reader.getLookahead()));
    reader.expect('e');
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
    EXPECT_EQ(reader.getColNum(), 1);
    reader.advance();
    reader.advance();
    reader.advance();
    EXPECT_EQ(reader.getLineNum(), 1);
    EXPECT_EQ(reader.getColNum(), 1);
}