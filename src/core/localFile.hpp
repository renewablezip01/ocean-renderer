#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

namespace core {
class LocalFile {

public:
  static std::string ReadFileData(const std::string &path) {
    std::stringstream buffer;
    std::string line;
    std::ifstream myfile(path);
    if (myfile.is_open()) {
      while (getline(myfile, line))
        buffer << line << '\n';
      myfile.close();
    }
    return buffer.str();
  }
};
} // namespace core
