
#include "BetaScope_Ana/SelfTrAna/AnaTemplate/SelfTrAna.h"

#include "General/utilities/include/dir.h"

#include <boost/thread.hpp>
#include <thread>
#include <TThread.h>
#include <TROOT.h>


void runAna( std::string fileName )
{
  SelfTrAna doAna(fileName);
  doAna.initialize();
  doAna.loopEvents();
  doAna.finalize();
}

int main(int argc, char **argv)
{
  ROOT::EnableThreadSafety();
  TThread::IsInitialized();
  ROOT::EnableImplicitMT(16);

  if( argc < 2 )
  {
    runAna( argv[1] );
    return 0;
  }

  auto files = BetaScope_Utilities::Dir::getFiles( argv[1], argv[2] );
  for( auto f : files )
  {
      runAna(f);
  }

  return 0;
}
