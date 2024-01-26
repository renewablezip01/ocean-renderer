#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

namespace core {
class LocalFile {

public:
  static std::string ReadFileData(const std::string &path);
};
} // namespace core
