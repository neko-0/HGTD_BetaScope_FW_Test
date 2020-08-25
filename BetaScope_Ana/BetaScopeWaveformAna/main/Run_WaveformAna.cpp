#include "BetaScopeWaveformAna/AnaTemplate/BetaScopeWaveformAna.h"
#include "General/utilities/include/dir.h"

#include <dirent.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>
namespace bpo = boost::program_options;

#include <TROOT.h>
#include <TThread.h>

// stackoverflow
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int get_ram(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int runAna(
  const std::string &fileName,
  const std::string &config = "WaveformAnaConfig.ini",
  const bool &skipWaveform = false,
  const bool &skim = false,
  const bool &mp = false
)
{
  TThread::Lock();
  BetaScopeWaveformAna doAna(fileName.c_str());
  if (skipWaveform){doAna.setWaveform(skipWaveform);}
  doAna.readWaveformConfig(config);
  doAna.skim_output = skim;
  doAna.internal_mp = mp;
  bool initialized = doAna.Initialize();
  TThread::UnLock();
  // doAna.run();
  if(initialized)
  {
    doAna.LoopEvents();
    doAna.Finalize();
    return 0;
  }
  else
  {
    return 1;
  }
}

int main(int argc, char **argv) {

  bpo::options_description desc("BetaScope analysis options.");
  bpo::variables_map vm;
  bpo::command_line_style::style_t style = bpo::command_line_style::style_t(
    bpo::command_line_style::unix_style|
    bpo::command_line_style::allow_long_disguise
  );
  desc.add_options()
  ("help,h", "help message.")
  ("config", bpo::value<std::string>(), "configuration file")
  ("skipWaveform", bpo::bool_switch()->default_value(false), "skipping waveform in output file.")
  ("skim", bpo::bool_switch()->default_value(false), "skim the output file.")
  ("mp", bpo::bool_switch()->default_value(false), "internal mp")
  ("thread,j", bpo::value<unsigned>()->default_value(std::thread::hardware_concurrency()), "using number of thread.")
  ;
  bpo::store(bpo::parse_command_line(argc, argv, desc, style), vm);
  bpo::notify(vm);

  if(vm.count("help")){
    std::cout << desc << std::endl;
    return 0;
  }

  if(vm.count("config"))
  {
    LOG_INFO(" Starting beta scope analysis");

    std::time_t main_time = std::time(nullptr);

    LOG_INFO("Preparation: Thread configuration.");
    unsigned numThreads = vm["thread"].as<unsigned>();
    if(numThreads!=1){numThreads = numThreads/2;}
    LOG_INFO("number of simultaneously processing files = " + std::to_string(numThreads) );

    ROOT::EnableThreadSafety();
    ROOT::EnableImplicitMT(numThreads);
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

    LOG_INFO(" Preparing workers.");

    BetaScopeWaveformAna my_ana;
    my_ana.readWaveformConfig(vm["config"].as<std::string>());

    std::vector<std::string> fileList = BetaScope_Utilities::Dir::getFiles(my_ana.rawFilesDir.c_str(), ".root");

    /*
    boost::asio::thread_pool pool(numThreads);
    for( auto file : fileList )
    {
      boost::asio::post(pool, boost::bind(runAna, file, vm["config"].as<std::string>(), vm["skipWaveform"].as<bool>(), vm["skim"].as<bool>(), vm["mp"].as<bool>()));
      //runAna(file, vm["config"].as<std::string>(), vm["skipWaveform"].as<bool>(), vm["skim"].as<bool>(), vm["mp"].as<bool>());
    }
    pool.join();
    */

    ///*
    #pragma omp parallel for num_threads(numThreads)
    for( std::size_t i = 0; i < fileList.size(); i++ )
    {
      runAna(fileList.at(i), vm["config"].as<std::string>(), vm["skipWaveform"].as<bool>(), vm["skim"].as<bool>(), vm["mp"].as<bool>());
    }
    //*/


    LOG_INFO("Finished. Time cost: " + std::to_string(std::time(nullptr) - main_time));

    return 0;
  }
  else
  {
    std::cout << "Please specify configuration file. see -help" << std::endl;
    return 1;
  }





  /*
  BetaScopeWaveformAna doAna_temp;
  doAna_temp.readWaveformConfig(argv[1]);
  std::vector<std::string> fileList = BetaScope_Utilities::Dir::getFiles(doAna_temp.rawFilesDir.c_str(), ".root");
  std::vector<boost::thread *> workers_dorm;
  for (std::size_t i = 0, max = fileList.size(); i < max; i++) {
    std::string config = argv[1];
    bool skipWaveform = false;
    if (argc == 3 && std::strcmp(argv[2], "--skipWaveform") == 0) {
      skipWaveform = true;
    } else if (argc == 3) {
      std::cout << "invalid flag " << argv[2] << std::endl;
      return 1;
    } else {
    }
    workers_dorm.push_back(
        new boost::thread(runAna, fileList.at(i), config, skipWaveform));
    if (workers_dorm.size() == concurentThreadsSupported) {
      for (std::size_t id = 0, currentNumWorkder = workers_dorm.size();
           id < currentNumWorkder; id++) {
        workers_dorm[id]->join();
        delete workers_dorm[id];
      }
      workers_dorm.clear();
    }
  }
  if (workers_dorm.size() != 0) {
    for (std::size_t id = 0, currentNumWorkder = workers_dorm.size();
         id < currentNumWorkder; id++) {
      workers_dorm[id]->join();
      delete workers_dorm[id];
    }
  }
  */
}
