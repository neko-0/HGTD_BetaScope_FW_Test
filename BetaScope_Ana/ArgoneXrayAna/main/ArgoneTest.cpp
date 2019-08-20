
#include "BetaScope_Ana/ArgoneXrayAna/AnaTemplate/ArgoneXrayAna.h"

#include "General/utilities/include/dir.h"

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
  if(argc != 3)return -1;

  ROOT::EnableThreadSafety();
  TThread::IsInitialized();
  ROOT::EnableImplicitMT(16);

  auto files = BetaScope_Utilities::Dir::getFiles( argv[1], argv[2] );
  for( auto f : files )
  {
      runAna(f);
  }

  return 0;
}
