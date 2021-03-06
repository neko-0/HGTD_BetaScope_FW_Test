#include "SelfTrigger/AnaTemplate/SelfTrigger.h"

#include <dirent.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#include <boost/thread.hpp>

#include <TROOT.h>
#include <TThread.h>


int main(int argc, char **argv) {

  std::time_t main_time = std::time(nullptr);

  SelfTrigger self_trigger( argv[1] );
  self_trigger.run();

  std::cout << "Run_SelfTrigger" << " Finished. Time cost: " + std::to_string(std::time(nullptr) - main_time) << std::endl;

  return 0;
}
