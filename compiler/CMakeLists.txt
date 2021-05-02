cmake_minimum_required(VERSION 3.19)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler
        src/lexer/lexer.cpp src/lexer/lexer.h src/parser/parser.cpp src/parser/parser.h src/main.cpp src/main.h src/lexer/Token.cpp src/lexer/Token.h)
