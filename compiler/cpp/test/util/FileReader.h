//
// Created by Marc on 30.05.2021.
//

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileReader {
private:
    const std::string folderPath;
public:
    explicit FileReader(): folderPath("../fest-files/") {}
    explicit FileReader(std::string folderPath): folderPath(std::move(folderPath)) {}
    std::string fileToString(const std::string& fileName);
};
