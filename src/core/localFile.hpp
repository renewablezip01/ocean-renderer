#pragma once
#include <string>

namespace core {
class LocalFile {

public:
  static std::string ReadFileData(const std::string &path);
};
} // namespace core
