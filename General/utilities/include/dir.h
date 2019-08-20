/*
some useful stuffs
*/

#ifndef BETACOPE_UTILITIES_DIR_H
#define BETACOPE_UTILITIES_DIR_H

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <dirent.h>

namespace BetaScope_Utilities
{
  class Dir
  {
    Dir(){};
    ~Dir(){};

    public:

      static std::vector<std::string> _getFiles( const char *directory, std::string pattern = "" ); //old method
      static std::vector<std::string> getFiles( std::string directory, std::string pattern = "" );
  };
}

#endif // BETACOPE_UTILITIES_DIR_H
