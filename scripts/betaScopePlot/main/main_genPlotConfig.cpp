#include "betaScopePlot/include/plotConfigMgr.h"

int main(int argc, char const *argv[])
{
  if( argc < 2)
  {
    PlotConfigMgr::GenerateRunInfo();
  }
  return 0;
}
