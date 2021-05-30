//
// Created by Marc on 27.05.2021.
//

#include <gtest/gtest.h>
#include "lexer/LexerTest.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LexerTestEnvironment);
    return RUN_ALL_TESTS();
}