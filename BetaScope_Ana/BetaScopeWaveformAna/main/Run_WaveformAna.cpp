#include "BetaScopeWaveformAna/AnaTemplate/BetaScopeWaveformAna.h"

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <thread>

#include <boost/thread.hpp>

#include <TThread.h>
#include <TROOT.h>


std::vector<std::string> getFiles( const char *directory )
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

void runAna( std::string fileName, std::string config="WaveformAnaConfig.ini", bool skipWaveform = false)
{
  TThread::Lock();
  BetaScopeWaveformAna doAna( fileName.c_str() );
  if( skipWaveform )doAna.setWaveform(skipWaveform);
  doAna.readWaveformConfig(config);
  doAna.Initialize();
  TThread::UnLock();
  //doAna.run();
  doAna.LoopEvents();
  doAna.Finalize();
}

int main( int argc, char **argv )
{

  ColorCout::Msg( "Run_WaveformAna", " Starting beta scope analysis" );

  std::time_t main_time = std::time(nullptr);

  ColorCout::Msg( "Preparation", " Thread configuration." );
  ROOT::EnableThreadSafety();
  ROOT::EnableImplicitMT(16);
  unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
  ColorCout::Msg( "Preparation", " Thread number "+std::to_string(concurentThreadsSupported) );

  ColorCout::Msg( "Preparation", " Preparing workers." );
  BetaScopeWaveformAna doAna_temp; doAna_temp.readWaveformConfig(argv[1]);
  std::vector<std::string> fileList = getFiles( doAna_temp.rawFilesDir.c_str() );
  std::vector<boost::thread *> workers_dorm;
  for(std::size_t i = 0, max = fileList.size(); i<max; i++)
  {
    std::string config = argv[1];
    bool skipWaveform = false;
    if( argc==3 && std::strcmp(argv[2], "--skipWaveform")==0 ){
      skipWaveform = true;
    }
    else if(argc==3){
      std::cout << "invalid flag " << argv[2] << std::endl;
      return 1;
    }
    else{}
    workers_dorm.push_back( new boost::thread(runAna, fileList.at(i), config, skipWaveform) );
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

  ColorCout::Msg( "Run_WaveformAna", " Finished. Time cost: " + std::to_string(std::time(nullptr)-main_time) );

  return 0;
}
