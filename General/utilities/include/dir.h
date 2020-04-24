/*
some useful stuffs
*/

#ifndef BETACOPE_UTILITIES_DIR_H
#define BETACOPE_UTILITIES_DIR_H

#include <dirent.h>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace BetaScope_Utilities {
class Dir {
  Dir(){};
  ~Dir(){};

public:
  static std::vector<std::string>
  _getFiles(const char *directory, std::string pattern = ""); // old method
  static std::vector<std::string> getFiles(std::string directory, std::string pattern = "");
};
} // namespace BetaScope_Utilities

#endif // BETACOPE_UTILITIES_DIR_H
