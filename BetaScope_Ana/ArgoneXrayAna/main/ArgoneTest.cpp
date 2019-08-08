
#include "BetaScope_Ana/ArgoneXrayAna/AnaTemplate/ArgoneXrayAna.h"

void runAna( std::string fileName )
{
  ArgoneXrayAna doAna(fileName);
  doAna.initialize();
  doAna.loopEvents();
  doAna.finalize();
}

int main(int argc, char **argv)
{
  runAna(argv[1]);
  return 0;
}
