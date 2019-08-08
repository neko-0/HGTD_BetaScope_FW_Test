#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "General/Colorful_Cout/include/Colorful_Cout.h"
#include <string>
#include <iostream>

class ArgoneXrayAna : public BetaScope_AnaFramework
{
  BetaScope beta_scope;

  std::string ifile;

  public:
    ArgoneXrayAna( std::string ifile )
    {
      std::cout << "Using user template: " << "BetaScopeWaveformAna" << std::endl;
      this->ifile = ifile;
    };
    ~ArgoneXrayAna(){};

    //required, user can add more to the existing methods;
    void initialize();
    void loopEvents();
    void finalize();

};

int runAnalysis(std::string ifile);
