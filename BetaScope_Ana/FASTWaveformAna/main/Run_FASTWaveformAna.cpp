#include "FASTWaveformAna/AnaTemplate/FASTWaveformAna.h"

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

  FASTWaveformAna fast_waveform( argv[1] );
  fast_waveform.run();

  std::cout << "Run_FASTWaveformAna" << " Finished. Time cost: " + std::to_string(std::time(nullptr) - main_time) << std::endl;

  return 0;
}
