//
// Created by Marc on 26.05.2021.
//

#include <gtest/gtest.h>
#include "Reader.h"

TEST(ReaderTests, ReaderMaxLookahead) {
    Reader reader = Reader("File input", 4);
    EXPECT_EQ(reader.getMaxLookahead(), "File");
}

TEST(ReaderTests, ReaderLookahead) {
    Reader reader = Reader("File input", 4);
    EXPECT_EQ(reader.getLookahead(), 'F');
}