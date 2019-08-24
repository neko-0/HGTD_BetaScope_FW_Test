///////////////////////////////
//                           //
// Argument class header     //
//                           //
///////////////////////////////

#ifndef BETACOPE_EXT_H
#define BETACOPE_EXT_H

#include "BetaScope_Class.h"

#include <TH1.h>


#endif // BETACOPE_H

class BetaScope_Ext : public BetaScope
{
  std::string class_name = "BetaScope_Ext";

  public:
    //TH1 for input
    TTreeReaderArray<TH1I> *iTree_TH1I_Array[numCh] = {};
    TTreeReaderArray<TH1F> *iTree_TH1F_Array[numCh] = {};
    TTreeReaderArray<TH1D> *iTree_TH1D_Array[numCh] = {};

    std::map<std::string, TTreeReaderArray<TH1I>*> iTree_TH1I_Array_Map;
    std::map<std::string, TTreeReaderArray<TH1F>*> iTree_TH1F_Array_Map;
    std::map<std::string, TTreeReaderArray<TH1D>*> iTree_TH1D_Array_Map;

    TTreeReaderValue<TH1I> *iTree_TH1I[numCh] = {};
    TTreeReaderValue<TH1F> *iTree_TH1F[numCh] = {};
    TTreeReaderValue<TH1D> *iTree_TH1D[numCh] = {};

    std::map<std::string, TTreeReaderValue<TH1I>*> iTree_TH1I_Map;
    std::map<std::string, TTreeReaderValue<TH1F>*> iTree_TH1F_Map;
    std::map<std::string, TTreeReaderValue<TH1D>*> iTree_TH1D_Map;

    //TH1 for output
    std::vector<TH1I> *oTree_TH1I_Array[numCh] = {};
    std::vector<TH1F> *oTree_TH1F_Array[numCh] = {};
    std::vector<TH1D> *oTree_TH1D_Array[numCh] = {};

    std::map<std::string, std::vector<TH1I>*> oTree_TH1I_Array_Map;
    std::map<std::string, std::vector<TH1F>*> oTree_TH1F_Array_Map;
    std::map<std::string, std::vector<TH1D>*> oTree_TH1D_Array_Map;

    TH1I *oTree_TH1I[numCh] = {};
    TH1F *oTree_TH1F[numCh] = {};
    TH1D *oTree_TH1D[numCh] = {};

    std::map<std::string, TH1I* > oTree_TH1I_Map;
    std::map<std::string, TH1F* > oTree_TH1F_Map;
    std::map<std::string, TH1D* > oTree_TH1D_Map;

    BetaScope_Ext(){ std::cout << this->class_name << std::endl; };
    ~BetaScope_Ext()
    {
      std::cout << this << " call destructor at " << objectLocation <<std::endl;
    };

    void fillEvent();

    template <typename dtype>
    bool buildBranch(std::string branchName );

    bool setBranch( std::string typeName, std::string key, std::string branchName);
};
