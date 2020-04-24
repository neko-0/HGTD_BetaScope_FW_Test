
#include "ArgoneXrayAna/AnaTemplate/ArgoneXrayAna.h"

#include "General/utilities/include/dir.h"

#include <TROOT.h>
#include <TThread.h>
#include <boost/thread.hpp>
#include <thread>

void runAna(std::string fileName) {
  ArgoneXrayAna doAna(fileName);
  doAna.Initialize();
  doAna.LoopEvents();
  doAna.Finalize();
}

int main(int argc, char **argv) {
  ROOT::EnableThreadSafety();
  TThread::IsInitialized();
  ROOT::EnableImplicitMT(16);

  if (argc < 2) {
    runAna(argv[1]);
    return 0;
  }

  auto files = BetaScope_Utilities::Dir::getFiles(argv[1], argv[2]);
  for (auto f : files) {
    runAna(f);
  }

  return 0;
}
