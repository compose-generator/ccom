set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

add_executable(${BINARY}
        main.cpp
        reader/ReaderTest.cpp
        lexer/LexerTest.cpp
        parser/ParserTest.cpp
        analyzer/AnalyzerTest.cpp
        interpreter/InterpreterTest.cpp
        util/LanguageDescriptor.cpp
        util/LanguageDescriptor.h
        util/FileReader.cpp
        util/FileReader.h)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

file(COPY test-files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})