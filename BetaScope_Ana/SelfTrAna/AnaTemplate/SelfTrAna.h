#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScopeExt_Class.h"
#include "BetaScope_Driver/include/BetaScopeExt_Templates.tpp"
#include "BetaScope_Driver/include/BetaScope_AnaFramework.h"
#include "General/Colorful_Cout/include/Colorful_Cout.h"
#include <string>
#include <iostream>

class SelfTrAna : public BetaScope_AnaFramework<BetaScope_Ext>
{
  std::string ifile;

  public:
    SelfTrAna( std::string ifile )
    {
      std::cout << "Using user template: " << "SelfTrAna" << std::endl;
      this->ifile = ifile;
      std::cout << "on file: " << ifile << std::endl;
    };
    ~SelfTrAna(){};

    //required, user can add more to the existing methods;
    void initialize();
    void loopEvents();
    void finalize(){
      beta_scope.get_ofile()->cd();
      SelfTrAna::_finalize();
    };
    void _finalize();

    std::vector<double> *w[16];
    std::vector<double> *t;

    std::vector<double> *pmax[16];
    std::vector<double> *tmax[16];
    std::vector<int> *max_indexing[16];

    std::vector<double> *pulseArea[16];
    std::vector<double> *negPmax[16];
    std::vector<double> *negTmax[16];

    //example of filling standalone histograms
    TH1D standAloneHisto = TH1D("standAloneHisto", "standAloneHisto", 100, 1 ,1);
    TH1D *standAloneHisto_ptr = 0;

    //template <typename o_type, typename i_type>
    //void copyTTreeReaderArrayToVector( o_type *output_v, TTreeReaderArray<i_type> *input_v);



};

int runAnalysis(std::string ifile);
