#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "BetaScope_Driver/include/BetaScope_Class.h"

#include <iostream>
#include <string>

class BetaScopeWaveformAna : public BetaScope_AnaFramework<BetaScope> {
  BetaScope beta_scope;

public:
  BetaScopeWaveformAna() {
    std::cout << "Using user template: "
              << "BetaScopeWaveformAna" << std::endl;
  };
  ~BetaScopeWaveformAna(){};

  // user define loopEvents
  void initialize(std::string ifile);
  void loopEvents();
  void finalize();
};

int runAnalysis(std::string ifile);
