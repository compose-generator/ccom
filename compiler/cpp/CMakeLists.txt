cmake_minimum_required(VERSION 3.19)

set(Name CCom)
project(${Name})

set(CMAKE_CXX_STANDARD 20)

add_executable(${Name}
        # Main code
        src/main.cpp
        src/main.h
        src/util/JSONParser.cpp
        src/util/JSONParser.h
        src/reader/Reader.cpp
        src/reader/Reader.h
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/Token.cpp
        src/lexer/Token.h
        src/parser/Parser.cpp
        src/parser/Parser.h
        src/parser/ast/TopLevelExprAST.cpp
        src/parser/ast/TopLevelExprAST.h
        src/parser/ast/NumberExprAST.cpp
        src/parser/ast/NumberExprAST.h
        src/parser/ast/StringExprAST.cpp
        src/parser/ast/StringExprAST.h
        src/parser/ast/IdentifierExprAST.cpp
        src/parser/ast/IdentifierExprAST.h
        src/parser/ast/StmtExprAST.cpp
        src/parser/ast/StmtExprAST.h
        src/parser/ast/StmtLstExprAST.cpp
        src/parser/ast/StmtLstExprAST.h
        src/parser/ast/KeyExprAST.cpp
        src/parser/ast/KeyExprAST.h
        src/parser/ast/IfBlockExprAST.cpp
        src/parser/ast/IfBlockExprAST.h
        src/parser/ast/PayloadExprAST.cpp
        src/parser/ast/PayloadExprAST.h
        src/parser/ast/HasStmtExprAST.cpp
        src/parser/ast/HasStmtExprAST.h
        src/parser/ast/CompStmtExprAST.cpp
        src/parser/ast/CompStmtExprAST.h
        src/parser/ast/ValueExprAST.cpp
        src/parser/ast/ValueExprAST.h
        src/parser/ast/ComBlockBlockExprAST.cpp
        src/parser/ast/ComBlockBlockExprAST.h
        src/parser/ast/ComLineBlockExprAST.cpp
        src/parser/ast/ComLineBlockExprAST.h
        src/parser/ast/SectionExprAST.cpp
        src/parser/ast/SectionExprAST.h
        src/parser/ast/ComBlockExprAST.cpp
        src/parser/ast/ComBlockExprAST.h
        src/parser/ast/ContentExprAST.cpp
        src/parser/ast/ContentExprAST.h
        src/parser/ast/ContentBlockExprAST.cpp
        src/parser/ast/ContentBlockExprAST.h
        src/parser/ast/ArbitraryExprAST.cpp
        src/parser/ast/ArbitraryExprAST.h
        src/parser/ast/BooleanExprAST.cpp
        src/parser/ast/BooleanExprAST.h
        src/analyzer/Analyzer.cpp
        src/analyzer/Analyzer.h
        src/interpreter/Interpreter.cpp
        src/interpreter/Interpreter.h
        src/exception/UnexpectedCharException.cpp
        src/exception/UnexpectedCharException.h

        #Tests
        src/reader/ReaderTests.cpp
)

# Load json lib
include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Load gtest lib
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/a3460d1aeeaa43fdf137a6adefef10ba0b59fe4b.zip)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${Name} gtest_main nlohmann_json::nlohmann_json)

include(GoogleTest)
gtest_discover_tests(${Name})
#add_test(
#        NAME ${Name}
#        COMMAND ${Name}
#)

install(TARGETS ${Name})