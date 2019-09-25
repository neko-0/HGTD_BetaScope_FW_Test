#include "utilities/include/dir.h"


std::vector<std::string> BetaScope_Utilities::Dir::_getFiles( const char *directory, std::string pattern )
{
  DIR *dir;
  struct dirent *ent;
  std::vector<std::string> content = {};
  if( (dir = opendir (directory)) != NULL )
  {
    /* print all the files and directories within directory */
    while( (ent = readdir (dir)) != NULL )
    {
      std::string ifileName = ent->d_name;
      if(ifileName.find(pattern)!=std::string::npos)// && !(rootfiles.at(ifileName).compare(".") == 0) && !(rootfiles.at(ifileName).compare("..") == 0) )
      {
        std::string file_dir = directory;
        std::string file_name = file_dir + "/" + ifileName;
        content.push_back( file_name );
      }
      //file_counter += 1;
    }
    closedir (dir);
  }

  /*std::cout << "contents in " << directory << std::endl;
  for( std::size_t i = 0, max = content.size(); i < max; i++)
  {
    std::cout << content[i] << std::endl;
  }
  */

  //dir-> ~DIR();
  //ent-> ~dirent();
  return content;
}

/*==============================================================================
New method
===============================================================================*/
std::vector<std::string> BetaScope_Utilities::Dir::getFiles( std::string directory, std::string pattern )
{
  namespace fs = std::experimental::filesystem;

  std::vector<std::string> content = {};

  // looping through all of the files under directory
  for( const auto & entry : fs::directory_iterator(directory.c_str()) )
  {
    // removing the directory part
    std::string my_fileName = entry.path();
    my_fileName.erase(0, my_fileName.find(directory)+directory.length() );

    // spliting pattern based on wildcard symbol *
    std::vector<std::string> pattern_fragment = {};
    std::string my_pattern_copy = pattern;
    std::string wildcard_delimiter = "*";
    int pos = 0;
    while( (pos = my_pattern_copy.find(wildcard_delimiter)) != std::string::npos )
    {
      std::string my_pattern_fragment = my_pattern_copy.substr(0, pos);
      my_pattern_copy.erase(0, pos+wildcard_delimiter.length() );
    }
    pattern_fragment.push_back( my_pattern_copy );

    bool matched = false;
    for( const auto &frag : pattern_fragment )
    {
      if( my_fileName.find(frag) != std::string::npos )matched=true;
      else{
        matched=false;
        break;
      }
    }

    if( matched )
    {
     std::string file_dir = directory;
     std::string file_name = file_dir + "/" + my_fileName;
     content.push_back( file_name );
    }
  }

  return content;
}
