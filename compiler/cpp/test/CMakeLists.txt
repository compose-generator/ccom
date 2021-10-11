set(BINARY ${CMAKE_PROJECT_NAME}_test)

add_executable(${BINARY}
        main.cpp
        ReaderTest.cpp
        LexerTest.cpp
        ParserTest.cpp
        AnalyzerTest.cpp
        InterpreterTest.cpp
        util/LanguageDescriptor.cpp
        util/LanguageDescriptor.h
        util/FileReader.cpp
        util/FileReader.h)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

file(COPY test-files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})