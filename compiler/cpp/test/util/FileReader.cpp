//
// Created by Marc on 30.05.2021.
//

#include "FileReader.h"

std::string FileReader::fileToString(const std::string& fileName) {
    std::ifstream file(folderPath + fileName);
    std::string content;
    if (file) {
        std::ostringstream stringStream;
        stringStream << file.rdbuf();
        content = stringStream.str();
    }
    return content;
}
