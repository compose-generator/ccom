/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <utility>

class LanguageDescriptor {
private:
  const std::string fileExtension;
  const std::string comLineIden;
  const std::string comBlockIdenOpen;
  const std::string comBlockIdenClose;

public:
  explicit LanguageDescriptor(std::string fileExtension, std::string comLineIden, std::string comBlockIdenOpen,
                              std::string comBlockIdenClose)
      : fileExtension(std::move(fileExtension)), comLineIden(std::move(comLineIden)),
        comBlockIdenOpen(std::move(comBlockIdenOpen)), comBlockIdenClose(std::move(comBlockIdenClose)) {}
  std::string getFileExtension();
  std::string getComLineIden();
  std::string getComBlockIdenOpen();
  std::string getComBlockIdenClose();
};