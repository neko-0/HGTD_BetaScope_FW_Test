#include "BetaScope_Ana/TestTemplate/AnaTemplate/TestTemplate.h"

int main(int argc, char **argv) {
  TestTemplate my_TestTemplate(argv[1]);
  my_TestTemplate.run();
}
