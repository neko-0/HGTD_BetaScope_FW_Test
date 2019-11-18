#include "plotConfigMgr.h"
#include <stdlib.h>

void Generate_Runlist_raw()
{
	system("ls stats_* > Runlist_raw.txt");
	printf(" \n");
	system("cat Runlist_raw.txt ");
	printf("\nRunlist_raw.txt is ready.\n");
	printf(" \n");
}

int main(int argc, char const *argv[])
{
  if( argc < 2)
  {
    Generate_Runlist_raw();
    GenerateRunInfo();
  }
  return 0;
}
