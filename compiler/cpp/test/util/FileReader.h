/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class FileReader {
private:
  std::string dirPath;

public:
  FileReader() : dirPath("../test-files/") {}
  explicit FileReader(std::string folderPath) : dirPath(std::move(folderPath)) {}
  std::string fileToString(const std::string &subDir, const std::string &fileName);
};