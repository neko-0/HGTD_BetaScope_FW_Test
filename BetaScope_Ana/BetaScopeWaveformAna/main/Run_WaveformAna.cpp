#include "BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/BetaScopeWaveformAna.h"

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <thread>

#include <boost/thread.hpp>

#include <TThread.h>
#include <TROOT.h>


std::vector<std::string> getFiles( const char *directory)
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
      if(ifileName.find(".root")!=std::string::npos)// && !(rootfiles.at(ifileName).compare(".") == 0) && !(rootfiles.at(ifileName).compare("..") == 0) )
      {
        std::string file_dir = directory;
        std::string file_name = file_dir + "/" + ifileName;
        content.push_back( file_name );
      }
      //file_counter += 1;
    }
    closedir (dir);
  }
  std::cout << "contents in " << directory << std::endl;
  for( std::size_t i = 0, max = content.size(); i < max; i++)
  {
    std::cout << content[i] << std::endl;
  }
  //dir-> ~DIR();
  //ent-> ~dirent();
  return content;
}

void runAna( std::string fileName, std::string config="WaveformAnaConfig.ini" )
{
  TThread::Lock();
  BetaScopeWaveformAna doAna( fileName.c_str() );
  doAna.readWaveformConfig(config);
  doAna.initialize();
  TThread::UnLock();
  doAna.loopEvents();
  doAna.finalize();
}

int main( int argc, char **argv )
{
  ROOT::EnableThreadSafety();
  unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
  BetaScopeWaveformAna doAna_temp; doAna_temp.readWaveformConfig(argv[1]);
  std::vector<std::string> fileList = getFiles( doAna_temp.rawFilesDir.c_str() );
  std::vector<boost::thread *> workers_dorm;
  for(std::size_t i = 0, max = fileList.size(); i<max; i++)
  {
    std::string config = argv[1];
    workers_dorm.push_back( new boost::thread(runAna, fileList.at(i), config) );
    if(workers_dorm.size() == concurentThreadsSupported)
    {
      for( std::size_t id=0, currentNumWorkder = workers_dorm.size(); id<currentNumWorkder; id++)
      {
        workers_dorm[id]->join();
        delete workers_dorm[id];
      }
      workers_dorm.clear();
    }
  }
  if(workers_dorm.size() !=0)
  {
    for( std::size_t id=0, currentNumWorkder = workers_dorm.size(); id<currentNumWorkder; id++)
    {
      workers_dorm[id]->join();
      delete workers_dorm[id];
    }
  }


  return 0;
}
