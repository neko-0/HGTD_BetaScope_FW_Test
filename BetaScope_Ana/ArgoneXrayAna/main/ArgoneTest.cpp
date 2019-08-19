
#include "BetaScope_Ana/ArgoneXrayAna/AnaTemplate/ArgoneXrayAna.h"

#include <boost/thread.hpp>
#include <thread>
#include <TThread.h>
#include <TROOT.h>


void runAna( std::string fileName )
{
  ArgoneXrayAna doAna(fileName);
  doAna.initialize();
  doAna.loopEvents();
  doAna.finalize();
}

int main(int argc, char **argv)
{
  ROOT::EnableThreadSafety();
  TThread::IsInitialized();
  ROOT::EnableImplicitMT(16);
  runAna(argv[1]);
  return 0;
}
