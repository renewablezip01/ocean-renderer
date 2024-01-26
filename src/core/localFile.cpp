#include "localFile.hpp"
#include <string>

std::string core::LocalFile::ReadFileData(const std::string &path) {
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
