#include "plotConfigMgr.h"
#include <stdlib.h>

#include <iostream>
#include <fmt/format.h>

int main(int argc, char const *argv[])
{
	if(argc == 2)
	{
		std::cout << fmt::format("Hello {}!\n", "world");
		return 0;
	}
  if( argc < 2)
  {
    GenerateRunInfo();
  }
  return 0;
}
