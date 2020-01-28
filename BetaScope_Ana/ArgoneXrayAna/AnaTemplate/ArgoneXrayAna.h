#include "BetaScope_Driver/include/BetaScopeExt_Class.h"
#include "BetaScope_Driver/include/BetaScopeExt_Templates.tpp"
#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "General/Colorful_Cout/include/Colorful_Cout.h"
#include <iostream>
#include <string>

class ArgoneXrayAna : public BetaScope_AnaFramework<BetaScope_Ext> {
  std::string ifile;

public:
  ArgoneXrayAna(std::string ifile) {
    std::cout << "Using user template: "
              << "ArgoneXrayAna" << std::endl;
    this->ifile = ifile;
    std::cout << "on file: " << ifile << std::endl;
  };
  ~ArgoneXrayAna(){};

  // required, user can add more to the existing methods;
  void Initialize();
  void LoopEvents();
  void Finalize() {
    beta_scope.GetOutFile()->cd();
    ArgoneXrayAna::_Finalize();
  };
  void _Finalize();

  std::vector<double> *w[16];
  std::vector<double> *t;

  std::vector<double> *pmax[16];
  std::vector<double> *tmax[16];
  std::vector<int> *max_indexing[16];

  std::vector<double> *pulseArea[16];
  std::vector<double> *negPmax[16];
  std::vector<double> *negTmax[16];

  // example of filling standalone histograms
  TH1D standAloneHisto = TH1D("standAloneHisto", "standAloneHisto", 100, 1, 1);
  TH1D *standAloneHisto_ptr = 0;

  // template <typename o_type, typename i_type>
  // void copyTTreeReaderArrayToVector( o_type *output_v,
  // TTreeReaderArray<i_type> *input_v);
};

int RunAnalysis(std::string ifile);
