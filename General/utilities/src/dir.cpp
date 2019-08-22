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

  for( const auto & entry : fs::directory_iterator(directory.c_str()) )
  {
     std::string tmp_fileName = entry.path();
     tmp_fileName.erase(0, tmp_fileName.find(directory)+directory.length() );
     if( tmp_fileName.find(pattern) != std::string::npos )
     {
       std::string file_dir = directory;
       std::string file_name = file_dir + "/" + tmp_fileName;
       content.push_back( file_name );
     }
  }

  return content;
}
