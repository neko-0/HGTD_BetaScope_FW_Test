#ifndef SELF_TRIGGER
#define SELF_TRIGGER

#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "BetaScope_Driver/include/BetaScopeExt_Class.h"
#include "BetaScope_Driver/include/BetaScopeExt_Templates.tpp"
#include <iostream>
#include <string>


class SelfTrigger : public BetaScope_AnaFramework<BetaScope_Ext>
{
private:
  // this is are required.
  std::string ifile;


  std::vector<double> *w2;
  std::vector<double> *t2;

  std::vector<double> *threshold;
  std::vector<double> *npulese;
  std::vector<double> *rate;

  double *timestamp;

public:

  SelfTrigger(){};

  SelfTrigger(std::string ifile) : ifile(ifile) {
    std::cout << "Using user template: "
              << "SelfTrigger" << std::endl;
  };

  ~SelfTrigger(){};

  // required, user can add more to the existing methods;
  void Initialize();
  void Analysis();
  void LoopEvents();
  void Finalize();

  void run() {
    Initialize();
    LoopEvents();
    Finalize();
  };

};

#endif //SELF_TRIGGER
