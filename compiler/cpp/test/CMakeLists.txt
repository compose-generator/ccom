add_executable(ccomctest
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

add_test(NAME ccomctest COMMAND ccomctest)

target_link_libraries(ccomctest PUBLIC ccomclib gtest)

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)