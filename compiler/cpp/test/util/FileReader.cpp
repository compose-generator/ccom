/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "FileReader.h"

std::string FileReader::fileToString(const std::string &subDir, const std::string &fileName) {
  std::ifstream file(dirPath + "/" + subDir + "/" + fileName);
  // Check if file is readable
  if (!file)
    throw std::runtime_error("Could not read test file");
  // Read file
  std::string content;
  std::ostringstream stringStream;
  stringStream << file.rdbuf();
  content = stringStream.str();
  return content;
}