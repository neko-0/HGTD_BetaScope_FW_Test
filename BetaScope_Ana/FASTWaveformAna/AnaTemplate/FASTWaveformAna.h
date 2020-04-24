#ifndef FAST_WAVEFORM_ANA
#define FAST_WAVEFORM_ANA

#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "BetaScope_Driver/include/BetaScopeExt_Class.h"
#include "BetaScope_Driver/include/BetaScopeExt_Templates.tpp"
#include <iostream>
#include <string>


class FASTWaveformAna : public BetaScope_AnaFramework<BetaScope_Ext>
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

  FASTWaveformAna(){};

  FASTWaveformAna(std::string ifile) : ifile(ifile) {
    std::cout << "Using user template: "
              << "FAST Waveform Ana" << std::endl;
  };

  ~FASTWaveformAna(){};

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

#endif //FAST_WAVEFORM_ANA
