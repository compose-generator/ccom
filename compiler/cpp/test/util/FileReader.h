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
    const std::string dirPath;
public:
    explicit FileReader(): dirPath("../fest-files/") {}
    explicit FileReader(std::string folderPath): dirPath(std::move(folderPath)) {}
    std::string fileToString(const std::string& subDir, const std::string& fileName);
};
